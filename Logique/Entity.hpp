
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>

#include "Action.hpp"

namespace Logique {

	class Entity {
	public:
		typedef std::shared_ptr<Entity> Ptr;
		typedef std::function< void (const Action&) > ActionFunctor;

		inline bool isAlive() {
			return foodCount_ >= 0; 
		}
		
		inline void setAddAction(const ActionFunctor& func) {
			addAction_ = func;
		}

	protected:
		std::function< void (const Action&) > addAction_;
		unsigned int foodCount_;
	};

}

#endif /* !ENTITY_HPP */

