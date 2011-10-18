
#ifndef BOARD_HPP
#define BOARD_HPP

#include <boost/thread.hpp>
#include <array>

#include "Square.hpp"
#include "Coord.hpp"

namespace Logique {
	enum { BOARD_SIZE = 20 };

	class Board : public std::array< std::array<Square, BOARD_SIZE >, BOARD_SIZE > {
	public:
		void lock() { mtx_.lock(); }
		void unlock() { mtx_.unlock(); }

		const Square& get(const Coord& pos) const { return at(pos.x)[pos.y]; }
		Square& get(const Coord& pos) { return at(pos.x)[pos.y]; }

		const Square& operator()(const Coord& pos) const { return at(pos.x)[pos.y]; }
		Square& operator()(const Coord& pos) { return at(pos.x)[pos.y]; }

		void dump() const {
			for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
				for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
					at(x)[y].dump();
					std::cout << " ";
				}
				std::cout << std::endl;
			}
		}

	private:
		boost::mutex mtx_;
	};

}

#endif