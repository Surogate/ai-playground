
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stack>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/pool/pool_alloc.hpp>

#include "Action.hpp"
#include "Coord.hpp"
#include "Square.hpp"
#include "ActionStore.hpp"
#include "EntityConstant.hpp"

namespace Logique  {

	class Board;

	class Entity : public boost::enable_shared_from_this<Entity> {
	public:
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

		virtual void getNewAction(unsigned int actionStart) = 0;
		virtual void eat(Board& board) = 0;
		virtual void reproduce(Board& board) = 0;

		virtual void initActionArray(Board& board);

		void goUp(Board& board);
		void goLeft(Board& board);
		void goRight(Board& board);
		void goDown(Board& board);
		void wait();

		EntityAction getLastAction() const;
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

		typedef std::stack< ActionStore, std::deque<ActionStore, boost::pool_allocator<ActionStore> > > ActionStoreStack;

		const Square::EntityContain _type;
		boost::array<unsigned int, ACTION_CONTAINER_SIZE> _timeArray;
		boost::array<boost::function< void () > , ACTION_CONTAINER_SIZE> _actionArray;
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
		DecisionTree::ReturnValue _lastCompute;
		Action _newAction;

	private:
		bool moveToThisLocation(Board& board, const Coord& newLoc);

		Entity& operator=(const Entity&) { return *this; }
	};

}

#endif /* !ENTITY_HPP */

