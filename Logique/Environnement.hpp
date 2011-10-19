
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

#include "Callback_Environnement.hpp"
#include "Square.hpp"
#include "Action.hpp"
#include "Entity.hpp"
#include "Board.hpp"

namespace Logique {

	class Environnement : public Callback_Environnement {
	public:
		enum {
			ODOURONDEATH = 3
		};

		typedef std::list< Action > ActionList;
		typedef std::map< const Entity*, Entity::Ptr > EntityPtrSet;
		typedef std::stack< Action > ActionTmpStack;

		Environnement();
		~Environnement();

		void run();
		void Environnement::test(int x, int y) {
			popOdour(Coord(x, y), 3);
			board_.dump();
		}
		void addAction(const Action& value);
		void setBaseTime(const boost::posix_time::time_duration& time);
		void addSheep(const Coord& loc);
		void addSheep(unsigned int num);
		unsigned int getSheepNUm() const;
		unsigned int getWolfNum() const;

	private:
		void unsafeInsertAction(const Action& value);
		void insertActionStack();
		void preRun();
		Action createBoardPlay();
		void boardPlay();
		void initEntity(std::shared_ptr<Entity> value);
		void spawnSheep();
		void onEntityDeath(const Entity& value);
		void popOdour(const Coord& loc, unsigned int power = ODOURONDEATH);
		void addOdour(int x, int y, unsigned int value);

		unsigned int sheepNum_;
		unsigned int wolfNum_;

		Board board_;
		boost::posix_time::time_duration baseTime_;
		EntityPtrSet entityList_;
		boost::mutex listMtx_;
		ActionList actionList_;
		ActionTmpStack actionTmpStack_;

		std::random_device randomD_;
		std::mt19937 gen_;
		std::uniform_int_distribution<unsigned int> distri_;
	};

}


#endif /* !ENVIRONNEMENT_HPP */

