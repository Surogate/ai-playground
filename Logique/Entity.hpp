
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stack>
#include <deque>
#include <utility>

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
	class Environnement;

	class Entity : public boost::enable_shared_from_this<Entity> {
	public:
		typedef boost::shared_ptr<Entity> Ptr;
		typedef boost::function< void (const Action&) > ActionFunctor;
		typedef boost::function< void (Entity&) > EntityFunctor;
		typedef boost::function< Square& (const Coord&) > GetSquareFunctor;
		typedef boost::function< int () > GetNumberSpeciesFunctor;
		typedef boost::function< bool (const Coord&) > PopEntityFunctor;
		typedef boost::function< bool (const float&) > ValidScoreFunctor;

		Entity(const Square::EntityContain& type, DecisionTree& tree);
		Entity(const Square::EntityContain& type, DecisionTree& tree, const EnvironnementGenetic::EntityGen& adn);
		
		Entity(const Entity& orig);
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

		void getNewAction(unsigned int actionStart);
		EntityAction computeAction();
		void sendXp(float power);
		void sendXpNot(float power);
		
		virtual void destroyMe(Environnement& env) = 0;
		virtual void eat(Board& board, Environnement& env) = 0;
		virtual void reproduce(Board& board, Environnement& env) = 0;
		virtual void genXp() = 0;
		virtual void initActionArray(Board& board, Environnement& env);

		void goUp(Board& board, Environnement& env);
		void goLeft(Board& board, Environnement& env);
		void goRight(Board& board, Environnement& env);
		void goDown(Board& board, Environnement& env);
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

		typedef std::deque< ActionStore, boost::pool_allocator< ActionStore > > ActionStoreDeque;
		typedef std::pair< unsigned int, boost::function< void () > > ActionPair;
		typedef boost::array< ActionPair, ACTION_CONTAINER_SIZE > ActionArray;

		const Square::EntityContain _type;
		ActionArray _actionArray;
		Coord _loc;

		ActionFunctor _add_action;
		EntityFunctor _onDeath;
		GetSquareFunctor _getSquare;
		PopEntityFunctor _popEntity;
		ValidScoreFunctor _validScore;

		ActionStoreDeque _actionStack;

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

		float SUPERSTEP;
		float GOODSTEP;
		float NEUTRALSTEP;
		float BADSTEP;

		DecisionTree& _tree;

	private:
		bool moveToThisLocation(Board& board, const Coord& newLoc, Environnement& env);

		Entity& operator=(const Entity&) { return *this; }
	};

}

#endif /* !ENTITY_HPP */

