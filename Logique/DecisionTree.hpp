
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include "ID3Class.hpp"
#include "Entity.hpp"

namespace Logique {

	class DecisionTree {
	public:
		typedef ID3::ID3Class6<int /*present*/, int /*haut*/, int /*gauche*/, int /*bas*/, int /*droit*/, Entity::EntityAction> ID3Algo;
		typedef ID3Algo::Answer Answer;
		DecisionTree();

		Entity::EntityAction computeAction(int present, int haut, int gauche, int bas, int droite);
	private:
		ID3Algo _id3;

		std::array<Entity::EntityAction, Entity::ACTION_CONTAINER_SIZE> _randomArray;
		float _moyenne;
	};

}

#endif /* !DECISIONTREE_HPP */