
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stack>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>

#include "Action.hpp"
#include "Coord.hpp"
#include "Square.hpp"

namespace Logique  {

	class Board;

	class Entity : public boost::enable_shared_from_this<Entity> {
	public:
		enum Constant{
			BASEFOODTIME = 12,
			BASEFOODDECREASE = 1,
			FOOD_MAX = 15,
			FOOD_REP_LIMIT_START = 16,
			FOOD_REP_LIMIT_END = 7,
			MOVE_TIME = 2
		};

		enum EntityAction {
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
			EAT,
			REPRODUCE,
			WAIT,
			ACTION_CONTAINER_SIZE
		};

		typedef boost::shared_ptr<Entity> Ptr;
		typedef boost::function< void (const Action&) > ActionFunctor;
		typedef boost::function< void (Entity&) > EntityFunctor;
		typedef boost::function< Square& (const Coord&) > GetSquareFunctor;
		typedef boost::function< int () > GetNumberSpeciesFunctor;
		typedef boost::function< bool (const Coord&) > PopEntityFunctor;
		typedef boost::function< bool (const float&) > ValidScoreFunctor;

		Entity(const Square::EntityContain& type);
		virtual ~Entity();

		void cleanVtable();

		void addFood(unsigned int value);
		bool isAlive() const;

		void setAddAction(const ActionFunctor& func);
		void setOnDeath(const EntityFunctor& func);
		void setGetSquare(const GetSquareFunctor& func);
		void setGetNumberSpecies(const GetNumberSpeciesFunctor& func);
		void setPopEntityFunctor(const PopEntityFunctor& func);
		void setValidScore(const ValidScoreFunctor& func);

		void setLocation(Coord loc);
		const Coord& getLocation() const;
		Action createFoodAction(unsigned int time = BASEFOODTIME, unsigned int value = BASEFOODDECREASE);
		void decreaseFood(unsigned int value);
		Square::EntityContain getType() const;

		virtual Action getNewAction() = 0;

		virtual void initActionArray(Board& board);

		void goUp(Board& board);
		void goLeft(Board& board);
		void goRight(Board& board);
		void goDown(Board& board);
		void wait();
		virtual void eat(Board& board) = 0;
		virtual void reproduce(Board& board) = 0;
		void generateNewAction();
		EntityAction getLastAction() const;
		const Square& getSquareSup(Coord loc, const Coord& dir);
		const Square& getSquareLess(Coord loc, const Coord& dir);
		int getIntFromSup(Coord loc, const Coord& dir); 
		int getIntFromLess(Coord loc, const Coord& dir);
		void reInitPerf();
		void setFood(unsigned int value);
		const float& getLastMoy() const;

	protected:
		inline bool addAction(const Action& value) {
			if (_add_action && _foodCount) 
				_add_action(value);
			return _foodCount != 0;
		}

		void dumpType();

		float computeMoy() const;
		
		struct ActionStore {
			unsigned int foodcount;
			Square present;
			Square up;
			Square left;
			Square down;
			Square right;

			EntityAction result;
			EntityAction last;

			ActionStore(unsigned int foodcount, const Square& _present_, const Square& _up_, const Square& _left_, const Square& _down_, const Square& _right_, EntityAction _result_, EntityAction _last_)
				: present (_present_), up(_up_), left(_left_), down(_down_), right(_right_), result(_result_), last(_last_)
			{}
		};

		typedef std::stack< ActionStore > ActionStoreStack;

		const Square::EntityContain _type;
		boost::array<Action, ACTION_CONTAINER_SIZE> _actionArray;
		Coord _loc;

		ActionFunctor _add_action;
		EntityFunctor _onDeath;
		GetSquareFunctor _getSquare;
		GetNumberSpeciesFunctor _getSpecieNumber;
		PopEntityFunctor _popEntity;
		ValidScoreFunctor _validScore;

		ActionStoreStack _actionStack;

		float _numberEat;
		float _numberRep;
		float _actual;
		float _numberTot;
		unsigned int _rep_limit;
		unsigned int _foodCount;
		float _lastMoy;
		EntityAction _lastAction;

	private:
		bool moveToThisLocation(Board& board, const Coord& newLoc);
	};

}

#endif /* !ENTITY_HPP */

