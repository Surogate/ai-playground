
#ifndef ACTION_HPP
#define ACTION_HPP

#include <functional>

namespace Logique {

	struct Action {
		typedef std::function< void () > Functor;

		Action() : _tickBeforeAction(), _action() 
		{}

		Action(unsigned int time, const Functor& func) : _tickBeforeAction(time), _action(func) 
		{}

		void operator()() {
			 _action();
		}

		unsigned int _tickBeforeAction;
		Functor _action;
	};

}

#endif /* !ACTION_HPP */

