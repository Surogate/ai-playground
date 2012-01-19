
#ifndef ACTION_HPP
#define ACTION_HPP

#include <boost/function.hpp>
#include <iostream>

namespace Logique {

	struct Action {
		typedef boost::function< void (unsigned int startTick) > Functor;

		Action() : _tickStart(0), _tickBeforeAction(0), _action() 
		{}

		Action(unsigned int time_start, unsigned int time_before, const Functor& func) 
			: _tickStart(time_start), _tickBeforeAction(time_before), _action(func) 
		{}

		Action(const Action& orig)
			: _tickStart(orig._tickStart), _tickBeforeAction(orig._tickBeforeAction), _action(orig._action)
		{
		}

		Action& operator=(const Action& orig) {
			if (this != &orig) {
				_tickStart = orig._tickStart;
				_tickBeforeAction = orig._tickBeforeAction;
				_action = orig._action;
			}
			return *this;
		}

		bool increment(unsigned int tick_passed) {
			if (tick_passed > 2) {
				std::cout << "caca" << std::endl;
			}

			_tickStart += tick_passed;
			if (_tickStart >= _tickBeforeAction) {
				_action(_tickStart - _tickBeforeAction);
				return true;
			}
			return false;
		}

		int tickBefore() const
		{
			return _tickBeforeAction - _tickStart;
		}

		unsigned int _tickStart;
		unsigned int _tickBeforeAction;
		Functor _action;
	};

}

#endif /* !ACTION_HPP */

