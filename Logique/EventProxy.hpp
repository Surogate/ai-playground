
#ifndef EVENTPROXY_HPP
#define EVENTPROXY_HPP

#include <deque>

#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "Environnement_Event.h"

namespace Logique {

	template <typename deque_type>
	class DequeProxy {
	public:
		DequeProxy(std::deque< deque_type >& stack, boost::mutex& mut)
		: _stack(&stack), _mut(&mut)
		{
			_mut->lock();
		}

		~DequeProxy()
		{
			if (_mut > 0)
				_mut->unlock();
		}

		typename std::deque< deque_type >::iterator begin()
		{
			return _stack->begin();
		}

		typename std::deque< deque_type >::const_iterator begin() const
		{
			return _stack->begin();
		}

		typename std::deque< deque_type >::iterator end() 
		{
			return _stack->end();
		}

		typename std::deque< deque_type >::const_iterator end() const
		{
			return _stack->end();
		}

		void clear() 
		{
			_stack->clear();
		}

		void foreach(const boost::function< void (const deque_type&) >& func)
		{
			BOOST_FOREACH(Environnement_Event ev, *_stack)
			{
				func(ev);
			}
		}

	private:
		std::deque< deque_type >* _stack;
		boost::mutex* _mut;

		DequeProxy& operator=(const DequeProxy& ) { return *this; }
		DequeProxy(const DequeProxy& orig)
		{
			_stack = orig._stack;
			_mut = 0;
		}
	};
}

#endif