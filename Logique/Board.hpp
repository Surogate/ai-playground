
#ifndef BOARD_HPP
#define BOARD_HPP

#include <boost/thread.hpp>
#include <array>

#include "Square.hpp"

namespace Logique {
	enum BoardSize { SIZE = 20 };

	class Board : public std::array< std::array<Square, SIZE>, SIZE> {
	public:
		void lock() { mtx_.lock(); }
		void unlock() { mtx_.unlock(); }

	private:
		boost::mutex mtx_;
	};

}

#endif