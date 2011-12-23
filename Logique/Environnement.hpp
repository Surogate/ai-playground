
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
#include <boost/random.hpp>
#include <boost/array.hpp>
#include <boost/random/random_device.hpp>

#include "Callback_Environnement.hpp"
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

		Environnement();
		~Environnement();

		void setSpawnSheep(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setSpawnSheep(func);
		}

		void setSpawnWolf(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setSpawnWolf(func);
		}

		void setOnEntityMove(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setOnEntityMove(func);
		}

		void setOnReproduce(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setOnReproduce(func);
		}

		void setOnEntityEat(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setOnEntityEat(func);
		}

		void setOnEntityDead(const Callback_Environnement::EntityFunctor& func) {
			Callback_Environnement::getInstance().setOnEntityDead(func);
		}

		void setOnBoardChange(const Callback_Environnement::BoardFunctor& func) {
			Callback_Environnement::getInstance().setOnBoardChange(func);
		}

		void run();
		void Environnement::test(int x, int y) {
			popOdour(Coord(x, y), 3);
			_board.dump();
		}
		void addAction(const Action& value);
		void setBaseTime(const boost::posix_time::time_duration& time);

		//spawn Entity function
		bool addSheep(const Coord& loc);
		bool popSheepNear(const Coord& loc);
		void addSheep(unsigned int num);
		
		bool addWolf(const Coord& loc);
		bool popWolfNear(const Coord& loc);
		void addWolf(unsigned int num);

		unsigned int getSheepNum() const;
		unsigned int getWolfNum() const;

		bool validPerf(const float& value, Square::EntityContain type);

		const EntityPtrSet& getEntityList() const;
		void lock();
		void unlock();

	private:
		void unsafeInsertAction(const Action& value);
		void insertActionStack();
		void preRun();
		Action createBoardPlay();
		Action createLog();
		void boardPlay();
		void doLog();
		void initEntity(boost::shared_ptr<Entity> value, const Coord& loc);
		void spawnSheep();
		void onEntityDeath(Entity& value);
		void popOdour(const Coord& loc, unsigned int power = ODOURONDEATH);
		void addOdour(int x, int y, unsigned int value);

		boost::array<unsigned int, Square::ENTITY_CONTAINER_SIZE> _entityNum;

		Board _board;
		boost::posix_time::time_duration _baseTime;
		EntityPtrSet _entityList;
		boost::mutex _listMtx;
		boost::mutex _attriMtx;
		ActionList _actionList;
		ActionTmpStack _actionTmpStack;

		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}


#endif /* !ENVIRONNEMENT_HPP */

