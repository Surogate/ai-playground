
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
			REPRODUCE_LIMIT = 13
		};

		Wolf();
		~Wolf();

		Action getNewAction();

		void initActionArray(Board& board);

		void eat(Board& board);
		void reproduce(Board& board);

		static void initExp();
		void sendXp();

		EntityAction computeAction();
		unsigned int evaluate() const;
		bool hasWolfNext();

		static DecisionTree _tree;
	};

}

#endif /* !WOLF_HPP */

