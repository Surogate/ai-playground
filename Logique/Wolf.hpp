
#ifndef WOLF_HPP
#define WOLF_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"
#include "EnvironnementGenetic.hpp"

namespace Logique {

	class Environnement;

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

		void initActionArray(Board& board, Environnement& env);

		void eat(Board& board, Environnement& env);
		void reproduce(Board& board, Environnement& env);
		void genXp();

		bool hasWolfNext();
		unsigned int evaluate() const;
	};

}

#endif /* !WOLF_HPP */

