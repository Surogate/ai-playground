
#ifndef COORD_HPP
#define COORD_HPP

#include <iostream>

struct Coord {
	int x;
	int y;

	Coord();
	Coord(unsigned int _x_, unsigned int _y_);
	Coord& operator=(const Coord& pos);
	void operator+=(const Coord& pos);
	void operator-=(const Coord& pos);
	Coord operator+(const Coord& pos) const;
	Coord operator-(const Coord& pos) const;
	bool operator==(const Coord& pos) const;
	bool operator!=(const Coord& pos) const;

	static const Coord DOWN;
	static const Coord RIGHT;
	static const Coord UP;
	static const Coord LEFT;
};

std::ostream& operator<<(std::ostream& cout, const Coord& value);

#endif /* !COORD_HPP */

