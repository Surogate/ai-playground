
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"

namespace Logique {

	class Sheep : public Entity {
	public:
		enum Constant {
			FOOD_GAIN = 4,
			EAT_TIME = 2,
			REPRODUCE_TIME = 14,
			REPRODUCE_COUNTER = 15
		};

		Sheep(DecisionTree& tree);
		~Sheep();

		void onDeath();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);
		void genXp();

		bool hasSheepNext();
		unsigned int evaluate() const;
	};

}

#endif /* !SHEEP_HPP */

