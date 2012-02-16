
#include <cmath>

#include "Entity.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Environnement.hpp"

namespace Logique {

	Entity::Entity(const Square::EntityContain& type, DecisionTree& tree)
		: _type(type), _loc(), _add_action(), _numberEat(0), _numberRep(0), _actual(0), _numberTot(0), _rep_limit(0), _foodCount(0), _lastMoy(0), _lastAction(WAIT), _tree(tree)
		, SUPERSTEP(0.25f), GOODSTEP(0.2f), NEUTRALSTEP(0.05f), BADSTEP(0.1f) 
	{}

	Entity::Entity(const Square::EntityContain& type, DecisionTree& tree, const EnvironnementGenetic::EntityGen& value )
		: _type(type), _loc(), _add_action(), _numberEat(0), _numberRep(0), _actual(0), _numberTot(0), _rep_limit(0), _foodCount(0), _lastMoy(0), _lastAction(WAIT), _tree(tree)
		, SUPERSTEP(value._superStep), GOODSTEP(value._goodStep), NEUTRALSTEP(value._neutralStep), BADSTEP(value._badStep)
	{}


	Entity::~Entity() 
	{}

	Entity::Entity(const Entity& orig)
		: _type(orig._type), _loc(orig._loc), _add_action(orig._add_action), _numberEat(orig._numberEat), _numberRep(orig._numberRep)
		, _actual(orig._actual), _numberTot(orig._numberTot), _rep_limit(orig._rep_limit), _foodCount(orig._foodCount), _lastMoy(orig._lastMoy),
		_lastAction(orig._lastAction), _tree(orig._tree)
	{}

	void Entity::cleanVtable() 
	{ _newAction = Action(); }

	void Entity::addFood(unsigned int value) {
		_foodCount += value;
		if (_foodCount > FOOD_MAX)
			_foodCount = FOOD_MAX;
	}

	bool Entity::isAlive() const {
		return _foodCount > 0;
	}

	void Entity::setAddAction(const Entity::ActionFunctor& func) 
	{ _add_action = func; }

	void Entity::setOnDeath(const Entity::EntityFunctor& func) 
	{ _onDeath = func; }

	void Entity::setGetSquare(const GetSquareFunctor& func) 
	{ _getSquare = func; }

	void Entity::setGetNumberSpecies(const GetNumberSpeciesFunctor& func) 
	{ reInitPerf(); }

	void Entity::setPopEntityFunctor(const PopEntityFunctor& func) 
	{ _popEntity = func; }

	void Entity::setValidScore(const ValidScoreFunctor& func) 
	{ _validScore = func; }

	void Entity::setLocation(Coord loc) 
	{ _loc = loc; }

	Action Entity::createFoodAction(unsigned int time, unsigned int value) 
	{
		Action food;

		food._tickStart = 0;
		food._tickBeforeAction = time;
		food._action = boost::bind(&Entity::decreaseFood, shared_from_this(), value);

		return food;
	}

	void Entity::decreaseFood(unsigned int value) 
	{
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

	void Entity::setFood(unsigned int value) 
	{ _foodCount = value; }

	const Coord& Entity::getLocation() const 
	{ return _loc; }

	Square::EntityContain Entity::getType() const 
	{ return _type; }

	EntityAction Entity::computeAction()
	{
		_actionStack.push_front(ActionStore(_foodCount, _loc, _getSquare));

		_tree.initInputArray(_actionStack.begin(), _actionStack.end());
		_actionStack.front().result = _tree.computeAction();
		_actual++;
		return _tree.getValue(_actionStack.front().result);
	}

	void Entity::getNewAction(unsigned int actionStart)
	{
		EntityAction act = computeAction();

		if (isAlive()) {
			_actionArray[act].second();
			_newAction._tickStart = actionStart;
			_newAction._tickBeforeAction = _actionArray[act].first;
			_add_action(_newAction);
		}

		genXp();
	}

	void Entity::sendXp(float power)
	{
		ActionStoreDeque::const_iterator it = _actionStack.begin();
		ActionStoreDeque::const_iterator ite = _actionStack.end();

		if (_actionStack.size())
		{
			while (it != ite && it + 5 != ite)
			{
				_tree.initInputArray(it, ite);
				_tree.train(it->result, power);
				++it;
			}
		}

		while (_actionStack.size() > 5)
		{ _actionStack.pop_back(); }

		_tree.generateTree();
	}

	void Entity::sendXpNot(float power) 
	{ sendXp(-1 * power); }

	void Entity::initActionArray(Board& board, Environnement& env) 
	{
		_actionArray[WAIT] = ActionPair(WAIT_TIME, boost::bind(&Entity::wait, this));
		_actionArray[MOVE_UP] = ActionPair(MOVE_TIME, boost::bind(&Entity::goUp, this, boost::ref(board), boost::ref(env)));
		_actionArray[MOVE_DOWN] = ActionPair(MOVE_TIME, boost::bind(&Entity::goDown, this, boost::ref(board), boost::ref(env)));
		_actionArray[MOVE_LEFT] = ActionPair(MOVE_TIME, boost::bind(&Entity::goLeft, this, boost::ref(board), boost::ref(env)));
		_actionArray[MOVE_RIGHT] = ActionPair(MOVE_TIME, boost::bind(&Entity::goRight, this, boost::ref(board), boost::ref(env)));

		_newAction = Action(0, 0, boost::bind(&Entity::getNewAction, shared_from_this(), _1));
	}

	void Entity::wait() 
	{ _lastAction = WAIT; }

	void Entity::goUp(Board& board, Environnement& env) 
	{
		if (moveToThisLocation(board, board.getValidValue(_loc + Coord::UP), env)) {
			_lastAction = MOVE_UP;
		}
	}

	void Entity::goLeft(Board& board, Environnement& env) {
		if (moveToThisLocation(board, board.getValidValue(_loc + Coord::LEFT), env)) {
			_lastAction = MOVE_LEFT;
		}
	}

	void Entity::goRight(Board& board, Environnement& env) 
	{
		if (moveToThisLocation(board, board.getValidValue(_loc + Coord::RIGHT), env)) {
			_lastAction = MOVE_RIGHT;
		}
	}

	void Entity::goDown(Board& board, Environnement& env) 
	{
		if (moveToThisLocation(board, board.getValidValue(_loc + Coord::DOWN), env)) {
			_lastAction = MOVE_DOWN;
		}
	}

	bool Entity::moveToThisLocation(Board& board, const Coord& newLoc, Environnement& env) 
	{
		if (!board(newLoc).hasEntity(_type)) {
			board.lock();
			board(_loc).hasEntity(_type, 0);
			board(newLoc).hasEntity(_type, this);
			board.unlock();
			env.addEvent(Environnement_Event::ENTITY_MOVE, *this, _type, _loc, newLoc);
			_loc = newLoc;
			return true;
		}
		return false;
	}

	Logique::EntityAction Entity::getLastAction() const 
	{ return _lastAction; }

	void Entity::reInitPerf() 
	{
		_numberEat = 0;
		_numberRep = 0;
		_actual = _actionStack.size();
		_numberTot = 10;
	}

	float Entity::computeMoy() const 
	{
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


