
#ifndef ENVIRONNEMENT_HPP
#define ENVIRONNEMENT_HPP

#include <list>
#include <memory>
#include <array>
#include <stack>
#include <random>
#include <set>
#include <map>


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
		enum {
			ODOURONDEATH = 3
		};

		typedef std::list< Action > ActionList;
		typedef std::map< const Entity*, Entity::Ptr > EntityPtrSet;
		typedef std::stack< Action > ActionTmpStack;

		typedef boost::function< void (const Entity&) > EntityFunctor;
		typedef boost::function< void (const Board&) > BoardFunctor;

		Environnement();

		void run();

		void addAction(const Action& value);

		void setBaseTime(const boost::posix_time::time_duration& time);
		void setSpawnSheep(const EntityFunctor& onSpawnSheep);
		void setSpawnWolf(const EntityFunctor& onSpawnWolf);
		void setOnEntityMove(const EntityFunctor& onEntityMove);
		void setOnReproduce(const EntityFunctor& onEntityMove);
		void setOnEntityEat(const EntityFunctor& onEntityEat);
		void setOnEntityDead(const EntityFunctor& onEntityDead);
		void setOnBoardChange(const BoardFunctor& onBoardChange);

	private:
		void unsafeInsertAction(const Action& value);
		void insertActionStack();
		void preRun();
		Action createBoardPlay();
		void boardPlay();
		void spawnSheep();
		void onEntityDeath(const Entity& value);
		void popOdour(const Coord& loc, unsigned int power = ODOURONDEATH);
		void setOdour(int x, int y, unsigned int value);

		Board board_;
		boost::posix_time::time_duration baseTime_;
		EntityPtrSet entityList_;
		boost::mutex listMtx_;
		ActionList actionList_;
		ActionTmpStack actionTmpStack_;

		std::random_device randomD_;
		std::mt19937 gen_;
		std::uniform_int_distribution<unsigned int> distri_;

		//functor
		BoardFunctor onBoardChange_;
		EntityFunctor onEntityDeath_;
	};

}


#endif /* !ENVIRONNEMENT_HPP */

