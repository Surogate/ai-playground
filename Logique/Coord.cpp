
#include "Coord.hpp"

Coord::Coord() : x(), y() 
{}

Coord::Coord(unsigned int _x_, unsigned int _y_) : x(_x_), y(_y_)
{}

std::ostream& operator<<(std::ostream& cout, const Coord& value) {
	cout << "x " << value.x << " y " << value.y;
	return cout;
}

