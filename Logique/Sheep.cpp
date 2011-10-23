
#include <iostream>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	DecisionTree Sheep::_tree;

	Sheep::Sheep() : Entity(Square::SHEEP), _numberEat(0)
	{}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, std::bind(&Entity::eat, shared_from_this(), std::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, std::bind(&Entity::reproduce, shared_from_this(), std::ref(board)));
	}

	Entity::EntityAction Sheep::computeAction() {
		return _tree.computeAction(
			_getSquare(_loc), 
			_getSquare(_loc - Coord::DOWN),
			_getSquare(_loc - Coord::RIGHT),
			_getSquare(_loc + Coord::DOWN),
			_getSquare(_loc + Coord::RIGHT));
	}

	Action Sheep::getNewAction() {
		return _actionArray[computeAction()];
	}

	void Sheep::eat(Board& board) {
		if (isAlive() && board(_loc).hasGrass()) {
			_lastAction = EAT;
			std::cout << "eat" << std::endl;
			board(_loc).hasGrass(false);
			_foodCount += FOOD_GAIN;
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Sheep::reproduce(Board& board) {
		if (isAlive()) {
			std::cout << "reproduce" << std::endl;
			_lastAction = REPRODUCE;
		}
		generateNewAction();
	}

}

