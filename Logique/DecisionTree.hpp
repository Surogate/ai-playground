
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/array.hpp>
#include "ID3Class.hpp"
#include "fann.h"
#include "fann_cpp.h"

#include "Entity.hpp"
#include "Square.hpp"

namespace Logique {

	class DecisionTree {
	public:
		DecisionTree();

		Entity::EntityAction computeAction(unsigned int foodcount, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite);
		void train(unsigned int foodcount, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, Entity::EntityAction result);
		void trainNot(unsigned int foodcount, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, Entity::EntityAction result);
		Entity::EntityAction randomAction();
		const float& getMoy() const;
		void sendMoy(float value);
		void generateTree();

	private:
		void initInputArray(unsigned int foodcount, Entity::EntityAction lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right);
		void initOutputArray(Entity::EntityAction val, float valBase, float valChoice);
		std::size_t initInputArray(float* val, const Square& s);
		FANN::neural_net _ann;
		boost::array<float, Entity::ACTION_CONTAINER_SIZE> _output;
		boost::array<float, 27> _input;

		float _moyenne;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */