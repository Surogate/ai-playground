
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"

namespace Logique {

	Wolf::Wolf() 
		: Entity(Square::WOLF), _numberEat(0)
	{}

	void Wolf::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, std::bind(&Entity::eat, shared_from_this(), std::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, std::bind(&Entity::reproduce, shared_from_this(), std::ref(board)));
	}

	Entity::EntityAction Wolf::computeAction() {
		//mettre l'id3 la dedans
		return EntityAction(_distri(_gen));
	}

	Action Wolf::getNewAction() {
		return _actionArray[computeAction()];
	}

	void Wolf::eat(Board& board) {
		if (isAlive() && board(_loc).hasSheep()) {
			_lastAction = EAT;
			std::cout << "eat" << std::endl;
			board(_loc).hasSheep(false);
			_foodCount += FOOD_GAIN;
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive()) {
			std::cout << "reproduce" << std::endl;
			_lastAction = REPRODUCE;
		}
		generateNewAction();
	}
}
