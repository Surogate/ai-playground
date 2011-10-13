
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

		Square() : title_(), odour_() {}

		inline bool hasGrass() { return title_ == GRASS 
			|| title_ == GRASS_SHEEP 
			|| title_ == GRASS_WOLF 
			|| title_ == GRASS_SHEEP_WOLF; }

		inline void setGrass() {
			if (title_ == NONE)
				title_ = GRASS;
			if (title_ == SHEEP)
				title_ = GRASS_SHEEP;
			if (title_ == WOLF)
				title_ = GRASS_WOLF;
			if (title_ == SHEEP_WOLF)
				title_ = GRASS_SHEEP_WOLF;
		}
	};

}

#endif /* !SQUARE_HPP */

