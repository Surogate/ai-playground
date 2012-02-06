
#ifndef SHEEP_HPP
#define SHEEP_HPP

#include "Entity.hpp"
#include "DecisionTree.hpp"
#include "EnvironnementGenetic.hpp"

namespace Logique {

	class Environnement;

	class Sheep : public Entity {
	public:
		enum Constant {
			GRASS_EATED = 5,
			FOOD_GAIN = 4,
			EAT_TIME = 2,
			REPRODUCE_TIME = 14,
			REPRODUCE_COUNTER = 15
		};

		Sheep(DecisionTree& tree);
		Sheep(DecisionTree& tree, const EnvironnementGenetic::EntityGen& adn);
		~Sheep();

		void onDeath();

		void initActionArray(Board& board, Environnement&);

		void eat(Board& board, Environnement& env);
		void reproduce(Board& board, Environnement& env);
		void genXp();

		bool hasSheepNext();
		unsigned int evaluate() const;
	};

}

#endif /* !SHEEP_HPP */

