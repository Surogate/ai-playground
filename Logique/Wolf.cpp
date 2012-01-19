
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Wolf::_tree;

	Wolf::Wolf() 
		: Entity(Square::WOLF)
	{}

	Wolf::~Wolf() 
	{
		onDeath();
	}

	void Wolf::onDeath() 
	{
		sendXpNot(1.f);
	}

	void Wolf::initActionArray(Board& board) 
	{
		Entity::initActionArray(board);
		_newAction = Action(0, 0, boost::bind(&Entity::getNewAction, shared_from_this(), _1));
		_actionArray[EAT] = boost::bind(&Wolf::eat, this, boost::ref(board));
		_timeArray[EAT] = EAT_TIME;
		_actionArray[REPRODUCE] = boost::bind(&Wolf::reproduce, this, boost::ref(board));
		_timeArray[REPRODUCE] = REPRODUCE_TIME;
	}

	EntityAction Wolf::computeAction() 
	{
		_actionStack.push(ActionStore(_foodCount, _lastCompute, _loc, _getSquare));
		DecisionTree::ReturnValue result = _tree.computeAction(_actionStack.top());
		_lastCompute = result;
		_actionStack.top().result = result;
		_actual++;
		return _tree.getValue(result);
	}

	void Wolf::sendXp(float power) 
	{
		while (_actionStack.size()) {
			_tree.train(_actionStack.top(), power);
			_actionStack.pop();
		}
		_tree.generateTree();
	}

	void Wolf::sendXpNot(float power) {
		while (_actionStack.size()) {
			_tree.trainNot(_actionStack.top(), power);
			_actionStack.pop();
		}
	}

	void Wolf::getNewAction(unsigned int actionStart) 
	{
		EntityAction act = computeAction();
		
		if (isAlive()) {
			_actionArray[act]();
			_newAction._tickStart = 0;
			_newAction._tickBeforeAction = _timeArray[act];
			if (!_newAction.increment(actionStart))
				_add_action(_newAction);
		}

		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (moy > 0) {
				_lastMoy = moy;
				if (_validScore(moy)) {
					sendXp(1.8f);
				} else {
					sendXp(1.4f);
				}
			} else {
				sendXpNot(0.5f);
			}
			reInitPerf();
		}
	}

	void Wolf::eat(Board& board) 
	{
		if (isAlive() && _getSquare(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			Entity* sheep =_getSquare(_loc).getEntity(Square::SHEEP);
			if (sheep) {
				sheep->setFood(0);
			}
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
		}
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive() && _rep_limit > REPRODUCE_COUNTER && hasWolfNext() && _popEntity(_loc) && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep += 2;
			_rep_limit = REPRODUCE_COUNTER / 2;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);			
		}
	}

	bool Wolf::hasWolfNext() {
		int up = _getSquare(_loc + Coord::UP);
		int left = _getSquare(_loc + Coord::LEFT);
		int down = _getSquare(_loc + Coord::DOWN);
		int right = _getSquare(_loc + Coord::RIGHT);
		return (up & Square::WOLF_MASK) 
			|| (left & Square::WOLF_MASK)
			|| (down & Square::WOLF_MASK)
			|| (right & Square::WOLF_MASK);
	}
}
