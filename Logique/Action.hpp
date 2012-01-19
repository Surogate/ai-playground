
#ifndef ACTION_HPP
#define ACTION_HPP

#include <boost/function.hpp>

namespace Logique {

	struct Action {
		typedef boost::function< void (unsigned int startTick) > Functor;

		Action() : _tickBeforeAction(), _action() 
		{}

		Action(unsigned int time_start, unsigned int time_before, const Functor& func) : _tickStart(time_start), _tickBeforeAction(time_before), _action(func) 
		{}

		bool increment(unsigned int tick_passed) {
			_tickStart += tick_passed;
			if (_tickStart >= _tickBeforeAction) {
				_action(_tickStart - _tickBeforeAction);
				return true;
			}
			return false;
		}

		int tickBefore() const
		{
			return _tickStart - _tickBeforeAction;
		}

		unsigned int _tickStart;
		unsigned int _tickBeforeAction;
		Functor _action;
	};

}

#endif /* !ACTION_HPP */

