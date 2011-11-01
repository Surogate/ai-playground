
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"

namespace Logique {

	class Sheep : public Entity {
	public:
		enum Constant {
			FOOD_GAIN = 2,
			EAT_TIME = 3,
			REPRODUCE_TIME = 16
		};

		Sheep();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);
		bool hasSheepNext();

		EntityAction computeAction();

		Action getNewAction();
		unsigned int evaluate() const;

		static DecisionTree _tree;
	};

}

#endif /* !SHEEP_HPP */

