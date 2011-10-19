
#include <iostream>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Environnement.hpp"
#include "Sheep.hpp"

namespace Logique {

	Environnement::Environnement()
		: _board(), baseTime_(), entityList_(), listMtx_(), actionList_(), actionTmpStack_()
		, randomD_(), gen_(randomD_), distri_(0, BOARD_SIZE - 1)
	{
		baseTime_ = boost::posix_time::seconds(1);
		addAction(createBoardPlay());
	}

	Environnement::~Environnement() {}

	void Environnement::preRun() {
		spawnSheep();
		insertActionStack();

		if (actionList_.empty()) {
			std::cout << "no action" << std::endl;
		}
	}

	void Environnement::run() {
		preRun();

		unsigned int tickwait;
		boost::posix_time::time_duration sleep_time;
		boost::posix_time::time_duration zero;
		ActionList::iterator executor;
		ActionList::iterator toDeleteEnd;
		ActionList::iterator end;
		boost::chrono::system_clock::time_point timer_start;
		boost::chrono::system_clock::time_point timer_end = boost::chrono::system_clock::now();
		boost::chrono::duration<double> sec;
		while (!actionList_.empty()) {
			timer_start = boost::chrono::system_clock::now();
			tickwait = actionList_.begin()->tickBeforeAction_ - static_cast<unsigned int>(sec.count());
			sleep_time =  baseTime_ * tickwait;
			if (sleep_time != zero) {
				boost::this_thread::sleep(sleep_time);
			}

			executor = actionList_.begin();
			end = actionList_.end();

			//on execute toute les action qui se sont deroulé pendant l'attente
			while (executor != end && executor->tickBeforeAction_ <= tickwait) {
				executor->action_();
				++executor;
			}

			toDeleteEnd = executor;

			//les action restante voient leurs temps avant execution diminuer
			while (executor != end) {
				executor->tickBeforeAction_ -= tickwait;
				++executor;
			}

			actionList_.erase(actionList_.begin(), toDeleteEnd);

			//on insere les nouvelle actions
			insertActionStack();

			//on calcule le temps qu'on a mis pour accomplir les operation, pour le soustraire plus tard
			timer_end = boost::chrono::system_clock::now();
			sec = (timer_end - timer_start) / (baseTime_.total_milliseconds() / 1000);
		}

		std::cout << "Simulation end" << std::endl;
	}

	void Environnement::setBaseTime(const boost::posix_time::time_duration& time) {
		baseTime_ = time;
	}

	void Environnement::addSheep(const Coord& loc) {}

	void Environnement::addSheep(unsigned int num) {}

	unsigned int Environnement::getSheepNum() const {
		return _sheepNum;
	}

	unsigned int Environnement::getWolfNum() const {
		return _wolfNum;
	}

	void Environnement::addAction(const Action& value) {
		listMtx_.lock(); // ##list lock
		actionTmpStack_.push(value);
		listMtx_.unlock(); // ## list unlock
	}

	void Environnement::unsafeInsertAction(const Action& value) {
		ActionList::iterator it = actionList_.begin();
		ActionList::iterator ite = actionList_.end();

		while (it != ite && it->tickBeforeAction_ < value.tickBeforeAction_) {
			++it;
		}
		actionList_.insert(it, value);
	}

	void Environnement::insertActionStack() {
		listMtx_.lock(); // ##list lock
		while (actionTmpStack_.size()) {
			unsafeInsertAction(actionTmpStack_.top());
			actionTmpStack_.pop();
		}
		listMtx_.unlock(); // ## list unlock
	}

	Action Environnement::createBoardPlay() {
		Action act;

		act.action_ = std::bind(&Environnement::boardPlay, this);
		act.tickBeforeAction_ = 1;
		return act;
	}

	void Environnement::boardPlay() {
		Coord grassSpawn;
		unsigned int odour_higher = 0;

		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				if (_board[x][y].odour_ > odour_higher && !_board[x][y].hasGrass()) {
					odour_higher = _board[x][y].odour_;
					grassSpawn.x = x;
					grassSpawn.y = y;
				}
				if (_board[x][y].odour_)
					_board[x][y].odour_--;
			}
		}

		if (!odour_higher) {
			grassSpawn = Coord(distri_(gen_), distri_(gen_));
		} else {
			_board.dump();
		}

		//std::cout << "try spawn grass on " << grassSpawn << std::endl;
		if (!_board(grassSpawn).hasGrass()) {
			//std::cout << "spawn grass on " << grassSpawn << std::endl;
			_board.lock();
			_board(grassSpawn).hasGrass(true);
			int value = _board(grassSpawn).getInt();
			//std::cout << "case value " << value << std::endl;
			_board.unlock();
			cb_onBoardChange(_board);
		}

		addAction(createBoardPlay());
	}

	void Environnement::initEntity(std::shared_ptr<Entity> value) {
		value->setAddAction(boost::bind(&Environnement::addAction, this, _1));
		value->setOnDeath(boost::bind(&Environnement::onEntityDeath, this, _1));
		entityList_[value.get()] = value;
		addAction(value->createFoodAction());
	}

	void Environnement::spawnSheep() {
		Coord loc;
		unsigned int limit = 0;

		do {
			loc = Coord(distri_(gen_), distri_(gen_));
		} while (_board(loc).hasSheep() && limit < 10);

		if (limit < 10) {
			std::cout << "spawn sheep at " << loc << std::endl;
			std::shared_ptr<Sheep> sheepPtr(new Sheep());

			sheepPtr->addFood(Sheep::FOODMAX);
			sheepPtr->setLocation(loc);
			initEntity(sheepPtr);

			_board.lock();
			_board(loc).hasSheep(true);
			_board.unlock();
			_sheepNum++;
			cb_onSheepSpawn(*sheepPtr);
			cb_onBoardChange(_board);
		}
	}

	void Environnement::onEntityDeath(const Entity& value) {
		cb_onEntityDeath(value);

		_board.lock();
		popOdour(value.getLocation());
		value.removeAtLoc(_board);
		_board.unlock();
		cb_onBoardChange(_board);
		EntityPtrSet::iterator it = entityList_.find(&value);
		if (it != entityList_.end()) {
			entityList_.erase(it);
		}
		_board.dump();
	}

	void Environnement::popOdour(const Coord& loc, unsigned int power) {
		
		for (unsigned int size = power; size > 0; size--) {
			int x_start = loc.x - size + 1;
			int y_start;
			int x_end = loc.x + size;
			int y_end = loc.y + size;

			while (x_start < x_end) {
				y_start = loc.y - size + 1;

				while (y_start < y_end) {
					addOdour(x_start, y_start, 1);
					y_start++;
				}

				x_start++;
			}
		}
		
	}

	void Environnement::addOdour(int x, int y, unsigned int value) {
		if (x >= 0 && x < BOARD_SIZE  && y >= 0 && y < BOARD_SIZE ) {
			_board[x][y].addOdour(value);
		}
	}
}
