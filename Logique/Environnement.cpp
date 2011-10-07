
#include <boost\thread.hpp>

#include "Environnement.hpp"

namespace Logique {

	Environnement::Environnement() 
		: board_(), threadWaiter_(0), entityList_(), actionList_()
	{
		baseTime_ = boost::posix_time::seconds(1);
	}

	void Environnement::run() {
	}

	void Environnement::setBaseTime(const boost::posix_time::time_duration& time) {
		baseTime_ = time;
	}

	void Environnement::waitUntil() {
		threadWaiter_ = new boost::thread(boost::bind(&Environnement::waitFunction, this));
		threadWaiter_ ->join();
	}

	void Environnement::waitFunction() {
		boost::this_thread::sleep(boost::system_time( boost::date_time::pos_infin ));
	}

	void Environnement::addAction(const Action& value) {
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
