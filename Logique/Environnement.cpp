
#include <iostream>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Environnement.hpp"

namespace Logique {

	Environnement::Environnement() 
		: board_(), entityList_(), actionList_()
	{
		baseTime_ = boost::posix_time::seconds(1);
	}

	void Environnement::preRun() {
		listMtx_.lock(); // ##list lock
		insertActionStack();
		listMtx_.unlock(); // ## list unlock

		if (actionList_.empty()) {
			std::cout << "no action" << std::endl;
		}
	}

	void Environnement::run() {
		preRun();

		unsigned int tickwait;
		boost::posix_time::time_duration sleep_time;
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
			boost::this_thread::sleep(sleep_time);

			listMtx_.lock(); // ##list lock
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

			//on insere les nouvelle actions
			insertActionStack();	
			listMtx_.unlock(); // ##list unlock

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
		actionTmpStack_.push(value);
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
		while (actionTmpStack_.size()) {
			unsafeInsertAction(actionTmpStack_.top());
			actionTmpStack_.pop();
		}
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
	}

}
