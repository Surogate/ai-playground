
#include "Board.hpp"

namespace Logique {

	Board::Board() 
		: boost::array< boost::array<Square, BOARD_SIZE >, BOARD_SIZE >(), _mtx(), _default()
	{}

	void Board::lock() { 
		_mtx.lock(); 
	}

	void Board::unlock() { 
		_mtx.unlock(); 
	}

	const Square& Board::get(int x, int y) const {
		if (coordValid(x, y))
			return at(x)[y];
		return at(getValidValue(x))[getValidValue(y)];
	}

	Square& Board::get(int x, int y) {
		if (coordValid(x, y))
			return at(x)[y];
		return at(getValidValue(x))[getValidValue(y)];
	}

	const Square& Board::get(const Coord& pos) const {
		return get(pos.x, pos.y);
	}

	Square& Board::get(const Coord& pos) { 
		return get(pos.x, pos.y);
	}

	const Square& Board::operator()(const Coord& pos) const { 
		return get(pos); 
	}

	Square& Board::operator()(const Coord& pos) { 
		return get(pos); 
	}

	void Board::dump() const {
		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				at(x)[y].dump();
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}

	void Board::dumpSheep() const {
		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				at(x)[y].dumpSheep();
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}

	int Board::getSquare(const Coord& pos) const {
		return get(pos).getInt();
	}

	Coord Board::getValidValue(int x, int y) const {
		return Coord(getValidValue(x), getValidValue(y));
	}

	Coord Board::getValidValue(const Coord& val) const {
		return getValidValue(val.x, val.y);
	}

	int Board::getValidValue(int val) const {
		if (val < 0)
			return BOARD_SIZE + val;
		else if (val >= BOARD_SIZE) {
			return val % BOARD_SIZE;
		}
		return val;
	}

	bool Board::coordValid(const Coord& pos) {
		return coordValid(pos.x, pos.y);
	}

	bool Board::coordValid(int x, int y) {
		return (x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE);
	}
}