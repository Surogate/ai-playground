
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

		Sheep();
		~Sheep();

		void onDeath();

		void initActionArray(Board& board);

		void getNewAction(unsigned int actionStart);
		void eat(Board& board);
		void reproduce(Board& board);

		//static void initExp();
		void sendXp(float power);
		void sendXpNot(float power);

		EntityAction computeAction();
		bool hasSheepNext();
		unsigned int evaluate() const;

		static DecisionTree _tree;
	};

}

#endif /* !SHEEP_HPP */

