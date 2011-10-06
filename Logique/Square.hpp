
#ifndef SQUARE_HPP
#define SQUARE_HPP

struct Square {
	enum Title {
		NONE,
		BLOCK,
		GRASS,
		SHEEP,
		WOLF
	};

	Title title_;
	unsigned int odour_;
};

#endif /* !SQUARE_HPP */

