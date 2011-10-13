
#ifndef COORD_HPP
#define COORD_HPP

struct Coord {
	unsigned int x;
	unsigned int y;

	Coord() : x(), y() {}
	Coord(unsigned int _x_, unsigned int _y_) : x(_x_), y(_y_) {} 
};

#endif