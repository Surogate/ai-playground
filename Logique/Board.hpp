
#ifndef BOARD_HPP
#define BOARD_HPP

#include <boost/thread.hpp>
#include <array>

#include "Square.hpp"
#include "Coord.hpp"
#include "Entity.hpp"

namespace Logique {
	enum { BOARD_SIZE = 20 };

	class Board : public std::array< std::array<Square, BOARD_SIZE >, BOARD_SIZE > {
	public:
		void lock();
		void unlock();

		const Square& get(const Coord& pos) const;
		Square& get(const Coord& pos);

		const Square& operator()(const Coord& pos) const;
		Square& operator()(const Coord& pos);
		
		int getSquare(const Coord& pos) const;

		void dump() const;

		static bool coordValid(const Coord& pos);
	private:
		boost::mutex _mtx;
	};

}

#endif