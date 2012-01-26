
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/array.hpp>
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
			SQUAREEXPANDSIZE = 4,
			SQUAREINTSIZE = 1,
			INPUTSIZE = INPUTFOODSIZE + INPUTLASTACTIONSIZE + 5 * SQUAREEXPANDSIZE + 41 * SQUAREINTSIZE,
			OUTPUTSIZE = ACTION_CONTAINER_SIZE,
			HIDDENSIZE = INPUTSIZE,
			HIDDENSIZE_2 = INPUTSIZE,
			LAYERNUM = 4
		};

		const float LEARNINGRATE;
		const float DECISIONSTEP;
		const float TRAINSTEP;

		typedef boost::array<float, OUTPUTSIZE> OutputArray;
		typedef boost::array<float, INPUTSIZE> InputArray;
		typedef OutputArray ReturnValue;

		DecisionTree();

		ReturnValue computeAction(const ActionStore& value);
		void train(const ActionStore& value, float power);
		void trainNot(const ActionStore& value, float power);
		void generateTree();
		EntityAction getValue(const ReturnValue& ret);
		unsigned int getActionNum();
		unsigned int getActionNeural();
		void clear();

	private:
		FANN::neural_net _ann;
		OutputArray _output;
		InputArray _input;

		unsigned int randomAction();
		unsigned int _numTotal;
		unsigned int _numNeural;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */