
#include "Coord.hpp"

const Coord Coord::DOWN = Coord(1, 0); 
const Coord Coord::RIGHT = Coord(0, 1);

Coord::Coord() : x(), y() 
{}

Coord::Coord(unsigned int _x_, unsigned int _y_) : x(_x_), y(_y_)
{}

Coord& Coord::operator+(const Coord& pos) {
	x += pos.x;
	y += pos.y;
	return *this;
}

Coord& Coord::operator-(const Coord& pos) {
	x -= pos.x;
	y -= pos.y;
	return *this;
}

std::ostream& operator<<(std::ostream& cout, const Coord& value) {
	cout << "x " << value.x << " y " << value.y;
	return cout;
}

