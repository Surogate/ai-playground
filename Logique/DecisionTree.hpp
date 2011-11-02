
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include "ID3Class.hpp"
#include "Entity.hpp"
#include "Square.hpp"

namespace Logique {

	class DecisionTree {
	public:
		typedef ID3::ID3Class10<bool /* useable */, bool /* hasgrass */, bool /* has sheep */, bool /* has wolf */, int /* odour */, int /*haut*/, int /*gauche*/, int /*bas*/, int /*droit*/, Entity::EntityAction> ID3Algo;
		typedef ID3Algo::Answer Answer;
		DecisionTree();

		Entity::EntityAction computeAction(const Square& present, int haut, int gauche, int bas, int droite);
		void addAction(const Square& present, int haut, int gauche, int bas, int droite, Entity::EntityAction result);
		Entity::EntityAction randomAction();
		const float& getMoy() const;
		void sendMoy(float value);
		std::size_t getSize() const;
		void generateTree();

	private:
		ID3Algo _id3;

		float _moyenne;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */