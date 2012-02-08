
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/array.hpp>
#include "fann.h"
#include "fann_cpp.h"
#include "Square.hpp"
#include "EntityConstant.hpp"
#include "EnvironnementGenetic.hpp"

namespace Logique {

	struct ActionStore;

	class DecisionTree {
	public:
		enum {
			INPUTFOODSIZE = 1,
			INPUTLASTACTIONSIZE = ACTION_CONTAINER_SIZE,
			SQUAREEXPANDSIZE = 4,
			SQUAREINTSIZE = 1,
			PRESENTPOSITION = INPUTFOODSIZE + 41 * SQUAREINTSIZE,
			INPUTSIZE = PRESENTPOSITION + (INPUTLASTACTIONSIZE + PRESENTPOSITION) * 4,
			OUTPUTSIZE = ACTION_CONTAINER_SIZE,
		};

		float LEARNINGRATE;
		float DECISIONSTEP;
		float TRAINSTEP;
		unsigned int HIDDENSIZE;
		unsigned int LAYERNUM;
		FANN::activation_function_enum ACTIVATIONFUNCHIDDEN;
		FANN::activation_function_enum ACTIVATIONFUNCOUTPUT;

		typedef boost::array<float, OUTPUTSIZE> OutputArray;
		typedef boost::array<float, INPUTSIZE> InputArray;
		typedef OutputArray ReturnValue;

		DecisionTree();
		DecisionTree(const EnvironnementGenetic::DecisionTreeGen& value);
		DecisionTree& operator=(const DecisionTree& orig);

		template <typename InputIterator>
		void initInputArray(InputIterator start, InputIterator end)
		{
			std::size_t i = 0;
			i += start->initArray(&_input[i]);
			start++;
			while (start != end && i < INPUTSIZE)
			{
				i += start->initArray(&_input[i], start->result);
				i += start->initArray(&_input[i]);
			}

			while (i < INPUTSIZE)
			{
				_input[i] = 0;
				++i;
			}
		}

		ReturnValue computeAction();
		void train(const OutputArray& value, float power);
		void trainNot(const OutputArray& value, float power);
		void generateTree();
		EntityAction getValue(const ReturnValue& ret);
		unsigned int getActionNum();
		unsigned int getActionNeural();
		void clear();

	private:
		FANN::neural_net _ann;
		OutputArray _output;
		InputArray _input;

		void createNeuralNet();
		unsigned int randomAction();

		unsigned int _numTotal;
		unsigned int _numNeural;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */