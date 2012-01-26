
#ifndef WOLF_HPP
#define WOLF_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"

namespace Logique {

	class Wolf : public Entity {
	public:
		enum Constant {
			FOOD_GAIN = 8,
			EAT_TIME = 3,
			REPRODUCE_TIME = 18,
			REPRODUCE_COUNTER = 15
		};

		Wolf(DecisionTree& tree);
		~Wolf();

		void onDeath();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);
		void genXp();

		bool hasWolfNext();
		unsigned int evaluate() const;
	};

}

#endif /* !WOLF_HPP */

