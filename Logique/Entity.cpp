
#include <cmath>

#include "Entity.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	Entity::Entity(const Square::EntityContain& type)
		: _type(type), _loc(), _add_action(), _numberEat(0), _numberRep(0), _actual(0), _numberTot(0), _foodCount(0), _lastAction()
	{}

	Entity::~Entity() 
	{
		std::cout << "entity delete" << std::endl;
	}

	void Entity::cleanVtable() {
		for (unsigned int i = 0; i < _actionArray.size(); i++) {
			_actionArray[i] = Action();
		}
	}

	void Entity::addFood(unsigned int value) {
		_foodCount += value;
		if (_foodCount > FOOD_MAX)
			_foodCount = FOOD_MAX;
	}

	bool Entity::isAlive() const {
		return _foodCount > 0;
	}

	void Entity::setAddAction(const Entity::ActionFunctor& func) {
		_add_action = func;
	}

	void Entity::setOnDeath(const Entity::EntityFunctor& func) {
		_onDeath = func;
	}

	void Entity::setGetSquare(const GetSquareFunctor& func) {
		_getSquare = func;
	}

	void Entity::setGetNumberSpecies(const GetNumberSpeciesFunctor& func) {
		_getSpecieNumber = func;
		reInitPerf();
	}

	void Entity::setPopEntityFunctor(const PopEntityFunctor& func) {
		_popEntity = func;
	}

	void Entity::setLocation(Coord loc) {
		_loc = loc;
	}

	Action Entity::createFoodAction(unsigned int time, unsigned int value) {
		Action food;

		food._tickBeforeAction = time;
		food._action = std::bind(&Entity::decreaseFood, this->shared_from_this(), value);

		return food;
	}

	void Entity::decreaseFood(unsigned int value) {
		if (value <= _foodCount)
			_foodCount -= value;
		else
			_foodCount = 0;

		if (_foodCount)
			addAction(createFoodAction());
		else if (_onDeath)
			_onDeath(*this);
	}

	const Coord& Entity::getLocation() const {
		return _loc;
	}

	Square::EntityContain Entity::getType() const {
		return _type;
	}

	void Entity::initActionArray(Board& board) {
		_actionArray[MOVE_UP] = Action(MOVE_TIME, std::bind(&Entity::goUp, shared_from_this(), std::ref(board)));
		_actionArray[MOVE_DOWN] = Action(MOVE_TIME, std::bind(&Entity::goDown, shared_from_this(), std::ref(board)));
		_actionArray[MOVE_LEFT] = Action(MOVE_TIME, std::bind(&Entity::goLeft, shared_from_this(), std::ref(board)));
		_actionArray[MOVE_RIGHT] = Action(MOVE_TIME, std::bind(&Entity::goRight, shared_from_this(), std::ref(board)));
	}


	void Entity::goUp(Board& board) {
		if (isAlive() && _loc.x > 0) {
			_lastAction = MOVE_UP;
			std::cout << "move up" << std::endl;
			Coord newLoc(_loc.x - 1, _loc.y);
			moveToThisLocation(board, newLoc);
		}
		generateNewAction();
	}

	void Entity::goLeft(Board& board) {
		if (isAlive() && _loc.y > 0) {
			_lastAction = MOVE_LEFT;
			std::cout << "move left" << std::endl;
			Coord newLoc(_loc.x, _loc.y - 1);
			moveToThisLocation(board, newLoc);
		}
		generateNewAction();
	}

	void Entity::goRight(Board& board) {
		if (isAlive() && _loc.y < BOARD_SIZE - 1) {
			_lastAction = MOVE_RIGHT;
			std::cout << "move right" << std::endl;
			Coord newLoc(_loc.x, _loc.y + 1);
			moveToThisLocation(board, newLoc);
		}
		generateNewAction();
	}

	void Entity::goDown(Board& board) {
		if (isAlive() && _loc.x < BOARD_SIZE - 1) {
			_lastAction = MOVE_DOWN;
			std::cout << "move down" << std::endl;
			Coord newLoc(_loc.x + 1, _loc.y);
			moveToThisLocation(board, newLoc);
		}
		generateNewAction();
	}

	bool Entity::moveToThisLocation(Board& board, const Coord& newLoc) {
		if (!board(newLoc).hasEntity(_type)) {
			board.lock();
			board(_loc).hasEntity(_type, 0);
			board(newLoc).hasEntity(_type, this);
			board.unlock();
			_loc = newLoc;
			Callback_Environnement::getInstance().cb_onEntityMove(*this);
			Callback_Environnement::getInstance().cb_onBoardChange(board);
			return true;
		}
		return false;
	}

	void Entity::generateNewAction() {
		if (isAlive()) _add_action(getNewAction());
	}

	Entity::EntityAction Entity::getLastAction() const {
		return _lastAction;
	}

	void Entity::reInitPerf() {
		_numberEat = 0;
		_numberRep = 0;
		_actual = 0;
		_numberTot = static_cast<float>(_getSpecieNumber());
	}

	float Entity::computeMoy() const {
		float moy = std::pow(_numberEat, _numberRep + 1.f);
		moy /= _actual;
		return moy;
	}
}


