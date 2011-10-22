
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <array>
#include <random>

#include "Action.hpp"
#include "Coord.hpp"
#include "Square.hpp"

namespace Logique  {

	class Board;

	class Entity : public std::enable_shared_from_this<Entity> {
	public:
		enum Constant{
			BASEFOODTIME = 3,
			BASEFOODDECREASE = 1,
			FOODMAX = 10,
			MOVE_TIME = 3
		};

		enum EntityAction {
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
			EAT,
			REPRODUCE,
			ACTION_CONTAINER_SIZE
		};

		typedef std::shared_ptr<Entity> Ptr;
		typedef std::function< void (const Action&) > ActionFunctor;
		typedef std::function< void (Entity&) > EntityFunctor;

		Entity(const Square::EntityContain& type);
		virtual ~Entity();

		void Entity::cleanVtable();

		void addFood(unsigned int value);
		bool isAlive() const;
		void setAddAction(const ActionFunctor& func);
		void setOnDeath(const EntityFunctor& func);
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
		virtual void eat(Board& board) = 0;
		virtual void reproduce(Board& board) = 0;
		void generateNewAction();
		EntityAction getLastAction() const;

	protected:
		inline bool addAction(const Action& value) {
			if (_add_action && _foodCount) 
				_add_action(value);
			return _foodCount != 0;
		}

		const Square::EntityContain _type;
		std::array<Action, ACTION_CONTAINER_SIZE> _actionArray;
		Coord _loc;
		ActionFunctor _add_action;
		EntityFunctor _onDeath;
		unsigned int _foodCount;
		EntityAction _lastAction;
	private:
		bool moveToThisLocation(Board& board, const Coord& newLoc);
	};

}

#endif /* !ENTITY_HPP */

