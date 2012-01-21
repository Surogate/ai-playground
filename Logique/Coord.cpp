
#include "Coord.hpp"

const Coord Coord::DOWN = Coord(1, 0); 
const Coord Coord::RIGHT = Coord(0, 1);
const Coord Coord::UP = Coord(-1, 0);
const Coord Coord::LEFT = Coord(0, -1);

Coord::Coord() : x(), y() 
{}

Coord::Coord(int _x_, int _y_) : x(_x_), y(_y_)
{}

Coord& Coord::operator=(const Coord& pos) {
	if (this != &pos) {
		x = pos.x;
		y = pos.y;
	}
	return *this;
}

void Coord::operator+=(const Coord& pos) {
	x += pos.x;
	y += pos.y;
}

void Coord::operator-=(const Coord& pos) {
	x -= pos.x;
	y -= pos.y;
}

Coord Coord::operator+(const Coord& pos) const {
	return Coord(x + pos.x, y + pos.y);
}

Coord Coord::operator-(const Coord& pos) const {
	return Coord(x - pos.x, y - pos.y);
}

bool Coord::operator==(const Coord& pos) const {
	return x == pos.x && y == pos.y;
}

bool Coord::operator!=(const Coord& pos) const {
	return !operator==(pos);
}

std::ostream& operator<<(std::ostream& cout, const Coord& value) {
	cout << "x " << value.x << " y " << value.y;
	return cout;
}

