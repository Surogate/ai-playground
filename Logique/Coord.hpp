
#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>

struct Coord {
	unsigned int x;
	unsigned int y;

	Coord();
	Coord(unsigned int _x_, unsigned int _y_);
};

std::ostream& operator<<(std::ostream& cout, const Coord& value);

#endif /* !COORD_HPP */

