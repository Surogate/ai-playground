
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>

#include "Action.hpp"
#include "Coord.hpp"
#include "Board.hpp"

namespace Logique  {

	class Entity : std::enable_shared_from_this<Entity> {
	public:
		enum {
			BASEFOODTIME = 3,
			BASEFOODDECREASE = 1,
			FOODMAX = 10
		};

		typedef std::shared_ptr<Entity> Ptr;
		typedef std::function< void (const Action&) > ActionFunctor;
		typedef std::function< void (const Entity&) > EntityFunctor;

		Entity();
		virtual ~Entity();

		void addFood(unsigned int value);
		bool isAlive() const;
		void setAddAction(const ActionFunctor& func);
		void setOnDeath(const EntityFunctor& func);
		void setLocation(Coord loc);
		const Coord& getLocation() const;
		Action createFoodAction(unsigned int time = BASEFOODTIME, unsigned int value = BASEFOODDECREASE);
		void decreaseFood(unsigned int value);

		virtual void removeAtLoc(Board& board) const = 0;

	protected:
		inline bool addAction(const Action& value) {
			if (addAction_ && foodCount_) 
				addAction_(value);
			return foodCount_ != 0;
		}

		Coord loc_;
		ActionFunctor addAction_;
		EntityFunctor onDeath_;
		unsigned int foodCount_;
	};

}

#endif /* !ENTITY_HPP */

