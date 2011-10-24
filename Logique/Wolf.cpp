
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	DecisionTree Wolf::_tree;

	Wolf::Wolf() 
		: Entity(Square::WOLF), _numberEat(0)
	{}

	void Wolf::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, std::bind(&Entity::eat, shared_from_this(), std::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, std::bind(&Entity::reproduce, shared_from_this(), std::ref(board)));
	}

	Entity::EntityAction Wolf::computeAction() {
		int present = _getSquare(_loc);
		int up = _getSquare(_loc - Coord::DOWN);
		int left = _getSquare(_loc - Coord::RIGHT);
		int down = _getSquare(_loc + Coord::DOWN);
		int right = _getSquare(_loc + Coord::RIGHT);
		EntityAction act = _tree.computeAction(present, up, left, down, right); 
		_actionStack.push(ActionStore(present, up, left, down, right, act));
		
		return act;
	}

	Action Wolf::getNewAction() {
		EntityAction act = computeAction();
		float moy = computeMoy();
		if (_actual && _actual >= _numberTot && computeMoy() >= _tree.getMoy()) {
			while (_actionStack.size()) {
				ActionStore& top = _actionStack.top();
				_tree.addAction(top.present, top.up, top.left, top.down, top.right, top.result);
				_actionStack.pop();
			}
			reInitPerf();
			_tree.sendMoy(moy);
		}
		return _actionArray[act];
	}

	void Wolf::eat(Board& board) {
		if (isAlive() && board(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			std::cout << "eat" << std::endl;
			board(_loc).hasSheep(false);
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive()) {
			_numberRep++;
			std::cout << "reproduce" << std::endl;
			_lastAction = REPRODUCE;
		}
		generateNewAction();
	}
}
