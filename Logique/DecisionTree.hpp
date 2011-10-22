
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
		Entity::EntityAction randomAction();

	private:
		ID3Algo _id3;

		float _moyenne;
		std::random_device _randomD;
		std::mt19937 _gen;
		std::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */