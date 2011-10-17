
#ifndef ACTION_HPP
#define ACTION_HPP

#include <functional>

namespace Logique {

	struct Action {
		unsigned int tickBeforeAction_;
		std::function< void () > action_;
	};

}

#endif /* !ACTION_HPP */

