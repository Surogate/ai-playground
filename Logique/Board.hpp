
#ifndef BOARD_HPP
#define BOARD_HPP

#include <boost/thread.hpp>

namespace Logique {

	template <typename TAB>
	class Board : public TAB {
	public:
		void lock() { mtx_.lock(); }
		void unlock() { mtx_.unlock(); }

	private:
		boost::mutex mtx_;
	};

}

#endif