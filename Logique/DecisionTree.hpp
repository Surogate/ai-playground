
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/array.hpp>
#include "ID3Class.hpp"
#include "fann.h"
#include "fann_cpp.h"
#include "Square.hpp"
#include "EntityConstant.hpp"

namespace Logique {

	struct ActionStore;

	class DecisionTree {
	public:
		enum {
			INPUTFOODSIZE = 1,
			INPUTLASTACTIONSIZE = ACTION_CONTAINER_SIZE,
			SQUAREEXPANDSIZE = 5,
			SQUAREINTSIZE = 1,
			INPUTSIZE = INPUTFOODSIZE + INPUTLASTACTIONSIZE + 5 * SQUAREEXPANDSIZE + 8 * SQUAREINTSIZE,
			OUTPUTSIZE = ACTION_CONTAINER_SIZE,
			HIDDENSIZE = INPUTSIZE / 2,
			LAYERNUM = 4
		};

		static const float DECISIONSTEP;
		static const float TRAINSTEP;

		typedef boost::array<float, OUTPUTSIZE> OutputArray;
		typedef boost::array<float, INPUTSIZE> InputArray;
		typedef OutputArray ReturnValue;

		DecisionTree();

		ReturnValue computeAction(const ActionStore& value);
		void train(const ActionStore& value);
		void trainNot(const ActionStore& value);
		const float& getMoy() const;
		void sendMoy(float value);
		void generateTree();
		EntityAction randomAction();
		EntityAction getValue(const ReturnValue& ret);

	private:
		FANN::neural_net _ann;
		OutputArray _output;
		InputArray _input;

		float _moyenne;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */