
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
		typedef typename deque_type::value_type value_type;

		DequeProxy(deque_type & stack, boost::mutex& mut)
		: _stack(&stack), _mut(&mut)
		{
			_mut->lock();
		}

		~DequeProxy()
		{
			if (_mut > 0)
				_mut->unlock();
		}

		typename deque_type::iterator begin()
		{
			return _stack->begin();
		}

		typename deque_type::const_iterator begin() const
		{
			return _stack->begin();
		}

		typename deque_type::iterator end() 
		{
			return _stack->end();
		}

		typename deque_type::const_iterator end() const
		{
			return _stack->end();
		}

		void clear() 
		{
			_stack->clear();
		}

		void foreach(const boost::function< void (const value_type&) >& func)
		{
			BOOST_FOREACH(Environnement_Event ev, *_stack)
			{
				func(ev);
			}
		}

		std::size_t size()
		{
			return _stack.size();
		}

	private:
		deque_type* _stack;
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