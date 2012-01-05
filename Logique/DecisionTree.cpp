
#include "DecisionTree.hpp"
#include "EntityConstant.hpp"
#include "ActionStore.hpp"

namespace Logique {

	const float DecisionTree::DECISIONSTEP = 0.15f;
	const float DecisionTree::TRAINSTEP = 0.08f;
	const float DecisionTree::LEARNINGRATE = 0.5f;

	DecisionTree::DecisionTree() 
		: _ann(), _moyenne(0.f)
		, _randomD(), _gen(_randomD), _distri(0, ACTION_CONTAINER_SIZE - 1)
	{
		_ann.create_standard(LAYERNUM, INPUTSIZE, HIDDENSIZE, HIDDENSIZE_2, OUTPUTSIZE);
		_ann.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		_ann.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		_ann.randomize_weights(0.3f, 0.7f);
		_ann.set_learning_rate(LEARNINGRATE);
	}

	EntityAction DecisionTree::randomAction() 
	{
		return static_cast<EntityAction>(_distri(_gen));
	}

	EntityAction DecisionTree::getValue(const ReturnValue& ret) 
	{
		std::size_t best;
		float val = ret[0];
		std::size_t i = 0;
		bool notClearAnswer = true;
		while (i < ACTION_CONTAINER_SIZE)
		{
			if (ret[i] >= val)
			{
				if (ret[i] - val > DECISIONSTEP)
					notClearAnswer = false;
				best = i;
				val = ret[i];
			}
			++i;
		}
		if (notClearAnswer)
			return randomAction();
		return static_cast<EntityAction>(best);
	}

	DecisionTree::ReturnValue DecisionTree::computeAction(const ActionStore& actionS) 
	{
		actionS.initArray(_input.c_array());
		float* output = _ann.run(_input.c_array());
		for (unsigned int i = 0; i < _output.size(); ++i)
			_output[i] = output[i];
		return _output;
	}

	const float& DecisionTree::getMoy() const 
	{
		return _moyenne;
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

	void DecisionTree::sendMoy(float value) 
	{
		_moyenne = (_moyenne + value) / 2.f;
	}

	void DecisionTree::generateTree() 
	{
	}
}


