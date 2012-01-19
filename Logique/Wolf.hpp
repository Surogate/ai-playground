
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

		Wolf();
		~Wolf();

		void initActionArray(Board& board);

		void getNewAction(unsigned int actionStart);
		void eat(Board& board);
		void reproduce(Board& board);

		//static void initExp();
		void sendXp(float power);
		void sendXpNot(float power);

		EntityAction computeAction();
		bool hasWolfNext();
		unsigned int evaluate() const;

		static DecisionTree _tree;
	};

}

#endif /* !WOLF_HPP */

