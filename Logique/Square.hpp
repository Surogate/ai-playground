
#ifndef SQUARE_HPP
#define SQUARE_HPP

namespace Logique {

	struct Square {
		enum Title {
			NONE,
			BLOCK,
			GRASS,
			SHEEP,
			WOLF,
			GRASS_SHEEP,
			GRASS_WOLF,
			GRASS_SHEEP_WOLF,
			SHEEP_WOLF
		};

		Title title_;
		unsigned int odour_;
	};

}

#endif /* !SQUARE_HPP */

