
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

	const Square& Board::get(const Coord& pos) const {
		if (coordValid(pos))
			return at(pos.x)[pos.y];
		return _default;
	}

	Square& Board::get(const Coord& pos) { 
		if (coordValid(pos))
			return at(pos.x)[pos.y];
		return _default;
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

	int Board::getSquare(const Coord& pos) const {
		if (Board::coordValid(pos)) {
			return get(pos).getInt();
		}
		return Square(false).getInt();
	}

	bool Board::coordValid(const Coord& pos) {
		return (pos.x < BOARD_SIZE && pos.y < BOARD_SIZE);
	}

	bool Board::coordValid(unsigned int x, unsigned int y) {
		return (x < BOARD_SIZE && y < BOARD_SIZE);
	}
}