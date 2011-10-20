
#ifndef WOLF_HPP
#define WOLF_HPP

#include "Entity.hpp"

namespace Logique {

	class Wolf {
	public:
		Action getNewAction() {}

		void removeAtLoc(Board& board) const {
			board(loc_).hasWolf(false);
		}
	};

}

#endif /* !WOLF_HPP */

