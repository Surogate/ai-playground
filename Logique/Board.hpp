
#ifndef BOARD_HPP
#define BOARD_HPP

#include <boost/thread.hpp>
#include <boost/array.hpp>

#include "Square.hpp"
#include "Coord.hpp"
#include "Entity.hpp"

namespace Logique {
	enum { BOARD_SIZE = 30 };

	class Board : public boost::array< boost::array<Square, BOARD_SIZE >, BOARD_SIZE > {
	public:
		Board();

		void lock();
		void unlock();

		const Square& get(const Coord& pos) const;
		Square& get(const Coord& pos);

		const Square& operator()(const Coord& pos) const;
		Square& operator()(const Coord& pos);
		
		int getSquare(const Coord& pos) const;

		void dump() const;

		static bool coordValid(const Coord& pos);
		static bool coordValid(unsigned int x, unsigned int y);
	private:
		boost::mutex _mtx;
		Square _default;
	};

}

#endif /* !BOARD_HPP */