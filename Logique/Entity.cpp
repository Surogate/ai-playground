
#include "Entity.hpp"

namespace Logique {

	Entity::Entity()
		: loc_(), addAction_(), foodCount_(0)
	{}

	Entity::~Entity() 
	{}

	void Entity::addFood(unsigned int value) {
		foodCount_ += value;
		if (foodCount_ > FOODMAX)
			foodCount_ = FOODMAX;
	}

	bool Entity::isAlive() const {
		return foodCount_ >= 0;
	}

	void Entity::setAddAction(const Entity::ActionFunctor& func) {
		addAction_ = func;
	}

	void Entity::setOnDeath(const Entity::EntityFunctor& func) {
		onDeath_ = func;
	}

	void Entity::setLocation(Coord loc) {
		loc_ = loc;
	}

	Action Entity::createFoodAction(unsigned int time, unsigned int value) {
		Action food;

		food.tickBeforeAction_ = time;
		food.action_ = std::bind(&Entity::decreaseFood, this->shared_from_this(), value);

		return food;
	}

	void Entity::decreaseFood(unsigned int value) {
		if (value <= foodCount_)
			foodCount_ -= value;
		else
			foodCount_ = 0;

		std::cout << "food at " << foodCount_ << std::endl;
		if (foodCount_)
			addAction(createFoodAction());
		else if (onDeath_)
			onDeath_(*this);
	}

	const Coord& Entity::getLocation() const {
		return loc_;
	}
}
