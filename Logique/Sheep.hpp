
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"

namespace Logique {

	class Sheep : public Entity {
	public:
		enum SheepAction {
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
			EAT,
			REPRODUCE
		};

		typedef std::function< void() > func;
		typedef std::map< SheepAction, Action > funcMap;

		Sheep() : Entity(), _actionMap() {}

		SheepAction computeAction() {
			return MOVE_UP;
		}

		Action getNewAction() {
			return _actionMap[computeAction()];
		}

		void removeAtLoc(Board& board) const {
			board(loc_).hasSheep(false);
		}

	private:
		funcMap _actionMap;
	};

}

#endif /* !SHEEP_HPP */

