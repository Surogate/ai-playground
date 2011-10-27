
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include "ID3Class.hpp"
#include "Entity.hpp"

namespace Logique {

	class DecisionTree {
	public:
		typedef ID3::ID3Class6<int /*present*/, int /*haut*/, int /*gauche*/, int /*bas*/, int /*droit*/, Entity::EntityAction> ID3Algo;
		typedef ID3Algo::Answer Answer;
		DecisionTree();

		Entity::EntityAction computeAction(int present, int haut, int gauche, int bas, int droite);
		void addAction(int present, int haut, int gauche, int bas, int droite, Entity::EntityAction result);
		Entity::EntityAction randomAction();
		const float& getMoy() const;
		void sendMoy(float value);

	private:
		ID3Algo _id3;

		float _moyenne;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */