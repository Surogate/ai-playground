
#include <iostream>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Environnement.hpp"

namespace Logique {

	Environnement::Environnement() 
		: board_(), baseTime_(), entityList_(), listMtx_(), actionList_(), actionTmpStack_(), randomD_(), gen_(randomD_), distri_(0, SIZE - 1)
	{
		baseTime_ = boost::posix_time::seconds(1);
		addAction(createBoardPlay());
	}

	void Environnement::preRun() {
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
				std::cout << "sleep_time " << sleep_time << std::endl;
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

		for (unsigned int x = 0; x < SIZE; ++x) {
			for (unsigned int y = 0; y < SIZE; ++y) {
				if (board_[x][y].odour_ > odour_higher) {
					odour_higher = board_[x][y].odour_;
					grassSpawn.x = x;
					grassSpawn.y = y;
				}
				if (board_[x][y].odour_)
					board_[x][y].odour_--;
			}
		}

		if (!odour_higher) {
			grassSpawn = Coord(distri_(gen_), distri_(gen_));
		}

		std::cout << "try spawn grass on " << grassSpawn << std::endl;
		if (!board_(grassSpawn).hasGrass()) {
			std::cout << "spawn grass on " << grassSpawn << std::endl;
			board_.lock();
			board_(grassSpawn).setGrass();
			board_.unlock();
			if (onBoardChange_) onBoardChange_(board_);
		}

		addAction(createBoardPlay());
	}

	void Environnement::setSpawnSheep(const boost::function< void (const Entity&) >& onSpawnSheep) {
	}

	void Environnement::setSpawnWolf(const boost::function< void (const Entity&) >& onSpawnWolf) {
	}

	void Environnement::setOnEntityMove(const boost::function< void (const Entity&) >& onEntityMove) {
	}

	void Environnement::setOnReproduce(const boost::function< void (const Entity&) >& onEntityMove) {
	}

	void Environnement::setOnEntityEat(const boost::function< void (const Entity&) >& onEntityEat) {
	}

	void Environnement::setOnEntityDead(const boost::function< void (const Entity&) >& onEntityDead) {
	}

	void Environnement::setOnBoardChange(const boost::function< void (const Board&) >& onBoardChange) {
		onBoardChange_ = onBoardChange;
	}

}
