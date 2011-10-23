
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include <cmath>
#include "Entity.hpp"
#include "DecisionTree.hpp"

namespace Logique {

	class Sheep : public Entity {
	public:
		enum Constant {
			FOOD_GAIN = 2,
			EAT_TIME = 3,
			REPRODUCE_TIME = 5
		};

		Sheep();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);

		EntityAction computeAction();

		Action getNewAction();
		unsigned int evaluate() const;

		static DecisionTree _tree;

	private:
		unsigned int _numberEat;


	};

}

#endif /* !SHEEP_HPP */

