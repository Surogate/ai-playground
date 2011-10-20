
#include <iostream>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Environnement.hpp"
#include "Sheep.hpp"

namespace Logique {

	Environnement::Environnement()
		: _board(), _baseTime(), _entityList(), _listMtx(), _attriMtx(), _actionList(), _actionTmpStack()
		, _randomD(), _gen(_randomD), _distri(0, BOARD_SIZE - 1)
	{
		_baseTime = boost::posix_time::seconds(1);
		addAction(createBoardPlay());
	}

	Environnement::~Environnement() {}

	void Environnement::preRun() {
		spawnSheep();
		insertActionStack();

		if (_actionList.empty()) {
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
		while (!_actionList.empty()) {
			timer_start = boost::chrono::system_clock::now();
			tickwait = _actionList.begin()->_tickBeforeAction - static_cast<unsigned int>(sec.count());
			sleep_time =  _baseTime * tickwait;
			if (sleep_time != zero) {
				boost::this_thread::sleep(sleep_time);
			}

			executor = _actionList.begin();
			end = _actionList.end();

			//on execute toute les action qui se sont deroul� pendant l'attente
			while (executor != end && executor->_tickBeforeAction <= tickwait) {
				executor->_action();
				++executor;
			}

			toDeleteEnd = executor;

			//les action restante voient leurs temps avant execution diminuer
			while (executor != end) {
				executor->_tickBeforeAction -= tickwait;
				++executor;
			}

			_actionList.erase(_actionList.begin(), toDeleteEnd);

			//on insere les nouvelle actions
			insertActionStack();

			//on calcule le temps qu'on a mis pour accomplir les operation, pour le soustraire plus tard
			timer_end = boost::chrono::system_clock::now();
			sec = (timer_end - timer_start) / (_baseTime.total_milliseconds() / 1000);
		}

		std::cout << "Simulation end" << std::endl;
	}

	void Environnement::setBaseTime(const boost::posix_time::time_duration& time) {
		_baseTime = time;
	}

	void Environnement::addSheep(const Coord& loc) {}

	void Environnement::addSheep(unsigned int num) {}

	unsigned int Environnement::getSheepNum() const {
		return _sheepNum;
	}

	unsigned int Environnement::getWolfNum() const {
		return _wolfNum;
	}

	const Environnement::EntityPtrSet& Environnement::getEntityList() const {
		return _entityList;
	}

	void Environnement::lock() {
		_attriMtx.lock();
	}

	void Environnement::unlock() {
		_attriMtx.unlock();
	}

	void Environnement::addAction(const Action& value) {
		_listMtx.lock(); // ##list lock
		_actionTmpStack.push(value);
		_listMtx.unlock(); // ## list unlock
	}

	void Environnement::unsafeInsertAction(const Action& value) {
		ActionList::iterator it = _actionList.begin();
		ActionList::iterator ite = _actionList.end();

		while (it != ite && it->_tickBeforeAction < value._tickBeforeAction) {
			++it;
		}
		_actionList.insert(it, value);
	}

	void Environnement::insertActionStack() {
		_listMtx.lock(); // ##list lock
		while (_actionTmpStack.size()) {
			unsafeInsertAction(_actionTmpStack.top());
			_actionTmpStack.pop();
		}
		_listMtx.unlock(); // ## list unlock
	}

	Action Environnement::createBoardPlay() {
		Action act;

		act._action = std::bind(&Environnement::boardPlay, this);
		act._tickBeforeAction = 1;
		return act;
	}

	void Environnement::boardPlay() {
		Coord grassSpawn;
		unsigned int odour_higher = 0;

		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				if (_board[x][y].odour() > odour_higher && !_board[x][y].hasGrass()) {
					odour_higher = _board[x][y].odour();
					grassSpawn.x = x;
					grassSpawn.y = y;
				}
				
				_board[x][y].decreaseOdour(1);
			}
		}

		if (!odour_higher) {
			grassSpawn = Coord(_distri(_gen), _distri(_gen));
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
			Callback_Environnement::getInstance().cb_onBoardChange(_board);
		}

		addAction(createBoardPlay());
	}

	void Environnement::initEntity(std::shared_ptr<Entity> value) {
		value->setAddAction(boost::bind(&Environnement::addAction, this, _1));
		value->setOnDeath(boost::bind(&Environnement::onEntityDeath, this, _1));
		_attriMtx.lock();
		_entityList[value.get()] = value;
		_attriMtx.unlock();
		addAction(value->createFoodAction());
		addAction(value->getNewAction());
	}

	void Environnement::spawnSheep() {
		Coord loc;
		unsigned int limit = 0;

		do {
			loc = Coord(_distri(_gen), _distri(_gen));
		} while (_board(loc).hasSheep() && limit < 10);

		if (limit < 10) {
			std::cout << "spawn sheep at " << loc << std::endl;
			std::shared_ptr<Sheep> sheepPtr(new Sheep());
			sheepPtr->initActionArray(_board);
			initEntity(sheepPtr);

			sheepPtr->addFood(Sheep::FOODMAX);
			sheepPtr->setLocation(loc);

			_board.lock();
			_board(loc).hasSheep(true);
			_board.unlock();
			_attriMtx.lock();
			_sheepNum++;
			_attriMtx.unlock();
			Callback_Environnement::getInstance().cb_onSheepSpawn(*sheepPtr);
			Callback_Environnement::getInstance().cb_onBoardChange(_board);
		}
	}

	void Environnement::onEntityDeath(Entity& value) {
		Callback_Environnement::getInstance().cb_onEntityDeath(value);

		_board.lock();
		popOdour(value.getLocation());
		_board(value.getLocation()).hasEntity(value.getType(), false);
		if (value.getType() == Square::SHEEP) {
			_attriMtx.lock();
			_sheepNum --;
			_attriMtx.unlock();
		}
		_board.unlock();
		Callback_Environnement::getInstance().cb_onBoardChange(_board);
		value.cleanVtable();
		EntityPtrSet::iterator it = _entityList.find(&value);
		if (it != _entityList.end()) {
			_attriMtx.lock();
			_entityList.erase(it);
			_attriMtx.unlock();
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
