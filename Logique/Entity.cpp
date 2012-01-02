
#include <cmath>

#include "Entity.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	Entity::Entity(const Square::EntityContain& type)
		: _type(type), _loc(), _add_action(), _numberEat(0), _numberRep(0), _actual(0), _numberTot(0), _rep_limit(0), _foodCount(0), _lastMoy(0), _lastAction(WAIT)
	{}

	Entity::~Entity() 
	{}

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

	void Entity::setValidScore(const ValidScoreFunctor& func) {
		_validScore = func;
	}

	void Entity::setLocation(Coord loc) {
		_loc = loc;
	}

	Action Entity::createFoodAction(unsigned int time, unsigned int value) {
		Action food;

		food._tickBeforeAction = time;
		food._action = boost::bind(&Entity::decreaseFood, this->shared_from_this(), value);

		return food;
	}

	void Entity::decreaseFood(unsigned int value) {
		if (value <= _foodCount)
			_foodCount -= value;
		else
			_foodCount = 0;

		_rep_limit++;
	
		if (_foodCount)
			addAction(createFoodAction());
		else if (_onDeath)
			_onDeath(*this);
	}

	void Entity::setFood(unsigned int value) {
		_foodCount = value;
	}

	const Coord& Entity::getLocation() const {
		return _loc;
	}

	Square::EntityContain Entity::getType() const {
		return _type;
	}

	void Entity::initActionArray(Board& board) {
		_actionArray[WAIT] = Action(1, boost::bind(&Entity::wait, shared_from_this()));
		_actionArray[MOVE_UP] = Action(MOVE_TIME, boost::bind(&Entity::goUp, shared_from_this(), boost::ref(board)));
		_actionArray[MOVE_DOWN] = Action(MOVE_TIME, boost::bind(&Entity::goDown, shared_from_this(), boost::ref(board)));
		_actionArray[MOVE_LEFT] = Action(MOVE_TIME, boost::bind(&Entity::goLeft, shared_from_this(), boost::ref(board)));
		_actionArray[MOVE_RIGHT] = Action(MOVE_TIME, boost::bind(&Entity::goRight, shared_from_this(), boost::ref(board)));
	}

	void Entity::wait() {
		if (isAlive())
		{
			_lastAction = WAIT;
		}
		generateNewAction();
	}

	void Entity::goUp(Board& board) {
		if (isAlive() && moveToThisLocation(board, board.getValidValue(_loc + Coord::UP))) {
			_lastAction = MOVE_UP;
		}
		generateNewAction();
	}

	void Entity::goLeft(Board& board) {
		if (isAlive() && moveToThisLocation(board, board.getValidValue(_loc + Coord::LEFT))) {
			_lastAction = MOVE_LEFT;
		}
		generateNewAction();
	}

	void Entity::goRight(Board& board) {
		if (isAlive() && moveToThisLocation(board, board.getValidValue(_loc + Coord::RIGHT))) {
			_lastAction = MOVE_RIGHT;
		}
		generateNewAction();
	}

	void Entity::goDown(Board& board) {
		if (isAlive() && moveToThisLocation(board, board.getValidValue(_loc + Coord::DOWN))) {
			_lastAction = MOVE_DOWN;
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

	Logique::EntityAction Entity::getLastAction() const {
		return _lastAction;
	}

	void Entity::reInitPerf() {
		_numberEat = 0;
		_numberRep = 0;
		_actual = 0;
		_numberTot = static_cast<float>(_getSpecieNumber()) / 1.5f + 1.f;
	}

	float Entity::computeMoy() const {
		if (_actual) {
			float moy = std::pow(_numberEat, _numberRep + 1.f);
			moy /= _actual;
			return moy;
		}
		return 0;
	}

	const float& Entity::getLastMoy() const {
		return _lastMoy;
	}

	void Entity::dumpType() {
		if (_type == Square::WOLF)
			std::cout << "wolf";
		if (_type == Square::SHEEP)
			std::cout << "sheep";
	}
}


