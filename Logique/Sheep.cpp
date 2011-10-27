
#include <iostream>
#include <cmath>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	DecisionTree Sheep::_tree;

	Sheep::Sheep() : Entity(Square::SHEEP)
	{}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, std::bind(&Entity::eat, shared_from_this(), std::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, std::bind(&Entity::reproduce, shared_from_this(), std::ref(board)));
	}

	Entity::EntityAction Sheep::computeAction() {
		Coord loc = _loc;
		int present = _getSquare(loc).getInt();
		int up = _getSquare(loc - Coord::DOWN).getInt();
		loc = _loc;
		int left = _getSquare(loc - Coord::RIGHT).getInt();
		loc = _loc;
		int down = _getSquare(loc + Coord::DOWN).getInt();
		loc = _loc;
		int right = _getSquare(loc + Coord::RIGHT).getInt();
		EntityAction act = _tree.computeAction(present, up, left, down, right); 
		_actionStack.push(ActionStore(present, up, left, down, right, act));
		_actual++;
		return act;
	}

	Action Sheep::getNewAction() {
		EntityAction act = computeAction();
		float moy = computeMoy();
		if (_actual && _actual >= _numberTot && moy && moy >= _tree.getMoy()) {
			while (_actionStack.size()) {
				ActionStore& top = _actionStack.top();
				_tree.addAction(top.present, top.up, top.left, top.down, top.right, top.result);
				_actionStack.pop();
			}
			_tree.sendMoy(moy);
			reInitPerf();
		}
		return _actionArray[act];
	}

	void Sheep::eat(Board& board) {
		if (isAlive() && board(_loc).hasGrass()) {
			_lastAction = EAT;
			_numberEat++;
			std::cout << "eat" << std::endl;
			board(_loc).hasGrass(false);
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Sheep::reproduce(Board& board) {
		if (isAlive() && hasSheepNext() && _popEntity(_loc)) {
			_numberRep++;
			std::cout << "reproduce" << std::endl;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().cb_onEntityReproduce(*this);
		}
		generateNewAction();
	}

	bool Sheep::hasSheepNext() {
		Coord loc = _loc;
		int up = _getSquare(loc - Coord::DOWN).getInt();
		loc = _loc;
		int left = _getSquare(loc - Coord::RIGHT).getInt();
		loc = _loc;
		int down = _getSquare(loc + Coord::DOWN).getInt();
		loc = _loc;
		int right = _getSquare(loc + Coord::RIGHT).getInt();
		return (up & Square::SHEEP_MASK) 
			|| (left & Square::SHEEP_MASK)
			|| (down & Square::SHEEP_MASK)
			|| (right & Square::SHEEP_MASK);
	}
}

