
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"

namespace Logique {

	class Sheep : public Entity {
	public:
		Action getNewAction() {}

		void removeAtLoc(Board& board) const {
			board(loc_).hasSheep(false);
		}
	};

}

#endif /* !SHEEP_HPP */

