
#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>

struct Coord {
	unsigned int x;
	unsigned int y;

	Coord();
	Coord(unsigned int _x_, unsigned int _y_);
	Coord& operator+(const Coord& pos);
	Coord& operator-(const Coord& pos);

	static const Coord DOWN;
	static const Coord RIGHT;
};

std::ostream& operator<<(std::ostream& cout, const Coord& value);

#endif /* !COORD_HPP */

