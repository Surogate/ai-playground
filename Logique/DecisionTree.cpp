
#include "DecisionTree.hpp"
#include "EntityConstant.hpp"
#include "ActionStore.hpp"

namespace Logique {

	DecisionTree::DecisionTree() 
		: _ann(), _numTotal(0), _numNeural(0)
		, _randomD(), _gen(_randomD), _distri(0, ACTION_CONTAINER_SIZE - 1)
		, DECISIONSTEP(0.1f), TRAINSTEP(0.05f), LEARNINGRATE(0.7f)
	{
		_ann.create_standard(LAYERNUM, INPUTSIZE, HIDDENSIZE, HIDDENSIZE_2, OUTPUTSIZE);
		_ann.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		_ann.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		_ann.set_learning_rate(LEARNINGRATE);
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
			i = 0;
			bool random_found = false;
			do {
				unsigned int rand = randomAction();
				if (best_val - ret[rand] < DECISIONSTEP) {
					best_pos = rand;
					random_found = true;
				}
				++i;
			} while (i < 10 && !random_found);
		} else 
		{
			_numNeural++;
		}

		
		return static_cast<EntityAction>(best_pos);
	}

	DecisionTree::ReturnValue DecisionTree::computeAction(const ActionStore& actionS) 
	{
		actionS.initArray(_input.c_array());
		float* output = _ann.run(_input.c_array());
		for (unsigned int i = 0; i < _output.size(); ++i)
			_output[i] = output[i];
		return _output;
	}

	void DecisionTree::train(const ActionStore& value, float power) 
	{
		value.initArray(_input.c_array());
		unsigned int i = 0;
		unsigned int choice = 0;
		float maxChoice = 0;
		_output = value.result;
		while (i < value.result.size()) {
			if (_output[i] >= maxChoice) {
				choice = i;
				maxChoice = value.result[i];
			}
			_output[i] -= TRAINSTEP * power;
			++i;
		}
		_output[choice] += TRAINSTEP * 2 * power;
		_ann.train(_input.c_array(), _output.c_array());
	}

	void DecisionTree::trainNot(const ActionStore& value, float power) 
	{
		value.initArray(_input.c_array());
		unsigned int i = 0;
		unsigned int choice = 0;
		float maxChoice = 0;
		_output = value.result;
		while (i < _output .size()) {
			if (_output[i] >= maxChoice) {
				choice = i;
				maxChoice = _output[i];
			}
			_output[i] += TRAINSTEP * power;
			++i;
		}
		_output[choice] -= TRAINSTEP * 2 * power;
		_ann.train(_input.c_array(), _output.c_array());
	}

	void DecisionTree::generateTree() 
	{
	}

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


