
#include <iostream>
#include <cmath>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Sheep::_tree;

	Sheep::Sheep() : Entity(Square::SHEEP)
	{}

	Sheep::~Sheep()
	{
		sendXpNot(1.f);
	}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_newAction = Action(0, 0, boost::bind(&Entity::getNewAction, shared_from_this(), _1));
		_actionArray[EAT] = boost::bind(&Sheep::eat, this, boost::ref(board));
		_timeArray[EAT] = EAT_TIME;
		_actionArray[REPRODUCE] = boost::bind(&Sheep::reproduce, this, boost::ref(board));
		_timeArray[REPRODUCE] = REPRODUCE_TIME;
	}

	EntityAction Sheep::computeAction() 
	{
		_actionStack.push(ActionStore(_foodCount, _lastCompute, _loc, _getSquare));
		DecisionTree::ReturnValue result = _tree.computeAction(_actionStack.top());
		_lastCompute = result;
		_actionStack.top().result = result;
		_actual++;
		return _tree.getValue(result);
	}

	void Sheep::sendXp(float power) 
	{
		while (_actionStack.size()) {
			_tree.train(_actionStack.top(), power);		
			_actionStack.pop();
		}

		_tree.generateTree();
	}

	void Sheep::sendXpNot(float power) 
	{
		while (_actionStack.size()) {
			_tree.trainNot(_actionStack.top(), power);
			_actionStack.pop();
		}

		_tree.generateTree();
	}

	void Sheep::getNewAction(unsigned int actionStart) 
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

	void Sheep::eat(Board& board) {
		if (board(_loc).hasGrass()) {
			_lastAction = EAT;
			_numberEat++;
			board.lock();
			board(_loc).hasGrass(false);
			board.unlock();
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
			Callback_Environnement::getInstance().addAction(Environnement_Event::GRASS_DOWN, _loc);
		}
	}

	void Sheep::reproduce(Board& board) {
		if (_rep_limit > REPRODUCE_COUNTER && hasSheepNext() && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep++;
			_rep_limit = REPRODUCE_COUNTER / 2;
			if (_numberRep < 2)
				_rep_limit += 2;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);
		}
	}

	bool Sheep::hasSheepNext() {
		int up = _getSquare(_loc + Coord::UP);
		int left = _getSquare(_loc + Coord::LEFT);
		int down = _getSquare(_loc + Coord::DOWN);
		int right = _getSquare(_loc + Coord::RIGHT);
		return (up & Square::SHEEP_MASK) 
			|| (left & Square::SHEEP_MASK)
			|| (down & Square::SHEEP_MASK)
			|| (right & Square::SHEEP_MASK);
	}
}

