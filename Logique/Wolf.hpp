
#ifndef WOLF_HPP
#define WOLF_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"

namespace Logique {

	class Wolf : public Entity {
	public:
		enum Constant {
			FOOD_GAIN = 2,
			EAT_TIME = 3,
			REPRODUCE_TIME = 5
		};

		Wolf();

		Action getNewAction();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);

		EntityAction computeAction();
		unsigned int evaluate() const;

		static DecisionTree _tree;

	private:
		unsigned int _numberEat;
	};

}

#endif /* !WOLF_HPP */

