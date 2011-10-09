
#ifndef ENVIRONNEMENT_HPP
#define ENVIRONNEMENT_HPP

#include <list>
#include <memory>
#include <array>
#include <stack>

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "Square.hpp"
#include "Action.hpp"
#include "Entity.hpp"
#include "Board.hpp"

namespace Logique {

	class Environnement {
	public:
		typedef std::list< Action > ActionList;
		typedef std::list< Entity::Ptr > EntityPtrList;
		typedef std::stack< Action > ActionTmpStack;

		Environnement();

		void run();

		void addAction(const Action& value);

		void setBaseTime(const boost::posix_time::time_duration& time);
		void setSpawnSheep(const boost::function< void (const Entity&) >& onSpawnSheep);
		void setSpawnWolf(const boost::function< void (const Entity&) >& onSpawnWolf);
		void setOnEntityMove(const boost::function< void (const Entity&) >& onEntityMove);
		void setOnReproduce(const boost::function< void (const Entity&) >& onEntityMove);
		void setOnEntityEat(const boost::function< void (const Entity&) >& onEntityEat);
		void setOnEntityDead(const boost::function< void (const Entity&) >& onEntityDead);
		void setOnBoardChange(const boost::function< void (const Board&) >& onBoardChange);

	private:
		void unsafeInsertAction(const Action& value);
		void insertActionStack();
		void preRun();

		Board board_;
		boost::posix_time::time_duration baseTime_;
		EntityPtrList entityList_;
		boost::mutex listMtx_;
		ActionList actionList_;
		ActionTmpStack actionTmpStack_;
	};

}


#endif /* !ENVIRONNEMENT_HPP */

