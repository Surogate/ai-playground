
#include "Board.hpp"

namespace Logique {

	void Board::lock() { 
		_mtx.lock(); 
	}

	void Board::unlock() { 
		_mtx.unlock(); 
	}

	const Square& Board::get(const Coord& pos) const {
		return at(pos.x)[pos.y]; 
	}

	Square& Board::get(const Coord& pos) { 
		return at(pos.x)[pos.y]; 
	}

	const Square& Board::operator()(const Coord& pos) const { 
		return at(pos.x)[pos.y]; 
	}

	Square& Board::operator()(const Coord& pos) { 
		return at(pos.x)[pos.y]; 
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
}