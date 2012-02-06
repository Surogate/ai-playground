
#include "DecisionTree.hpp"
#include "EntityConstant.hpp"
#include "ActionStore.hpp"

namespace Logique {

	DecisionTree::DecisionTree() 
		: _ann(), _numTotal(0), _numNeural(0)
		, _randomD(), _gen(_randomD), _distri(0, ACTION_CONTAINER_SIZE - 1)
		, DECISIONSTEP(0.15f), TRAINSTEP(0.05f), LEARNINGRATE(0.7f)
		, LAYERNUM(2), HIDDENSIZE(INPUTSIZE), ACTIVATIONFUNCHIDDEN(FANN::GAUSSIAN_STEPWISE), ACTIVATIONFUNCOUTPUT(FANN::GAUSSIAN_STEPWISE)
	{
		createNeuralNet();
		_ann.set_training_algorithm(FANN::TRAIN_INCREMENTAL);
		_ann.set_activation_function_hidden(ACTIVATIONFUNCHIDDEN);
		_ann.set_activation_function_output(ACTIVATIONFUNCOUTPUT);
		_ann.set_learning_rate(LEARNINGRATE);
	}

	DecisionTree::DecisionTree(const EnvironnementGenetic::DecisionTreeGen& value) 
		: _ann(), _numTotal(0), _numNeural(0)
		, _randomD(), _gen(_randomD), _distri(0, ACTION_CONTAINER_SIZE - 1)
		, DECISIONSTEP(value._decisionStep), TRAINSTEP(value._trainStep), LEARNINGRATE(value._learningRate)
		, LAYERNUM(value._layerNum), HIDDENSIZE(value._layerSize), ACTIVATIONFUNCHIDDEN(value._activationHidden), ACTIVATIONFUNCOUTPUT(value._activationOutput)
	{
		createNeuralNet();
		_ann.set_training_algorithm(FANN::TRAIN_INCREMENTAL);
		_ann.set_activation_function_hidden(ACTIVATIONFUNCHIDDEN);
		_ann.set_activation_function_output(ACTIVATIONFUNCOUTPUT);
		_ann.set_learning_rate(LEARNINGRATE);
	}

	void DecisionTree::createNeuralNet() 
	{
		if (LAYERNUM == 1)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 2)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 3)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 4)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 5)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 6)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 7)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
		if (LAYERNUM == 8)
		{
			_ann.create_standard(LAYERNUM + 2, INPUTSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, HIDDENSIZE, OUTPUTSIZE);
		}
	}

	unsigned int DecisionTree::randomAction() 
	{
		return _distri(_gen);
	}

	EntityAction DecisionTree::getValue(const ReturnValue& ret) 
	{
		std::size_t best_pos = 0;
		float best_val = ret[0];
		std::size_t i = 0;
		bool notClearAnswer = true;
		while (i < ACTION_CONTAINER_SIZE)
		{
			if (ret[i] > best_val)
			{
				if (ret[i] - best_val > DECISIONSTEP)
					notClearAnswer = false;
				else
					notClearAnswer = true;
				best_val = ret[i];
				best_pos = i;
			}
			++i;
		}

		_numTotal++;
		if (notClearAnswer) 
		{
			best_pos = randomAction();
		} 
		else 
		{
			_numNeural++;
		}

		return static_cast<EntityAction>(best_pos);
	}

	DecisionTree::ReturnValue DecisionTree::computeAction() 
	{
		float* output = _ann.run(_input.c_array());
		for (unsigned int i = 0; i < _output.size(); ++i)
			_output[i] = output[i];
		return _output;
	}

	void DecisionTree::train(const OutputArray& value, float power) 
	{
		unsigned int i = 0;
		unsigned int choice = 0;
		float maxChoice = 0;
		_output = value;
		while (i < value.size()) {
			if (_output[i] >= maxChoice) {
				choice = i;
				maxChoice = value[i];
			}
			_output[i] -= TRAINSTEP * power;
			++i;
		}
		_output[choice] += TRAINSTEP * 2 * power;
		_ann.train(_input.c_array(), _output.c_array());
	}

	void DecisionTree::trainNot(const OutputArray& value, float power) 
	{
		train(value, -1 * power);
	}

	void DecisionTree::generateTree() 
	{}

	unsigned int DecisionTree::getActionNum() {
		return _numTotal;
	}

	unsigned int DecisionTree::getActionNeural() {
		return _numNeural;
	}

	void DecisionTree::clear() {
		_numTotal = 0;
		_numNeural = 0;
	}
}


