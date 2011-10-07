
#ifndef ENVIRONNEMENT_HPP
#define ENVIRONNEMENT_HPP

#include <list>
#include <memory>
#include <array>

#include <boost/function.hpp>
#include <boost/date_time.hpp>

#include "Square.hpp"
#include "Action.hpp"
#include "Entity.hpp"
#include "Board.hpp"

namespace Logique {

	class Environnement {
	public:
		enum { SIZE = 20 };

		typedef std::array<Square, SIZE> ArrayBoard;
		typedef std::array<ArrayBoard, SIZE> ArrayArrayBoard;
		typedef Board< ArrayArrayBoard > Board;

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
		void waitUntil();
		void waitFunction();

		Board board_;
		boost::thread* threadWaiter_;

		boost::posix_time::time_duration baseTime_;
		std::list< std::shared_ptr<Entity> > entityList_;
		std::list< Action > actionList_;
	};

}


#endif /* !ENVIRONNEMENT_HPP */

