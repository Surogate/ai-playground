
#include "DecisionTree.hpp"
#include "EntityConstant.hpp"

namespace Logique {

	DecisionTree::DecisionTree() 
		: _ann(), _moyenne(0.f)
		, _randomD(), _gen(_randomD), _distri(0, ACTION_CONTAINER_SIZE - 1)
	{
		_ann.create_standard(3, 27, 27, ACTION_CONTAINER_SIZE);
		_ann.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		_ann.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		_ann.randomize_weights(0.3f, 0.7f);
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
				if (ret[i] - val > 0.1f)
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

	DecisionTree::ReturnValue DecisionTree::computeAction(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		float* output = _ann.run(_input.c_array());
		for (unsigned int i = 0; i < _output.size(); ++i)
			_output[i] = output[i];
		return _output;
	}

	const float& DecisionTree::getMoy() const 
	{
		return _moyenne;
	}

	void DecisionTree::train(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, ReturnValue result) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		unsigned int i = 0;
		unsigned int choice = 0;
		float maxChoice = 0;
		while (i < result.size()) {
			if (result[i] >= maxChoice) {
				choice = i;
				maxChoice = result[i];
			}
			result[i] -= 0.1f;
			++i;
		}
		result[choice] += 0.2f;
		_ann.train(_input.c_array(), result.c_array());
	}

	void DecisionTree::trainNot(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, ReturnValue result) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		unsigned int i = 0;
		unsigned int choice = 0;
		float maxChoice = 0;
		while (i < result.size()) {
			if (result[i] >= maxChoice) {
				choice = i;
				maxChoice = result[i];
			}
			result[i] += 0.1f;
			++i;
		}
		result[choice] -= 0.2f;
		_ann.train(_input.c_array(), result.c_array());
	}

	void DecisionTree::sendMoy(float value) 
	{
		_moyenne = (_moyenne + value) / 2.f;
	}

	void DecisionTree::generateTree() 
	{
	}


	void DecisionTree::initInputArray(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right)
	{
		std::size_t i = 0;
		_input[i] = static_cast<float>(foodcount / (Logique::FOOD_MAX));
		i++;
		i += initArray(&_input[i], lastAction);
		i += initArray(&_input[i], present);
		i += initArray(&_input[i], up);
		i += initArray(&_input[i], left);
		i += initArray(&_input[i], down);
		i += initArray(&_input[i], right);
	}

	std::size_t DecisionTree::initArray(float* tab, const Square& s)
	{
		std::size_t i = 0;

		tab[i] = static_cast<float>(s.useable());
		i++;
		tab[i] = static_cast<float>(s.hasGrass());
		i++;
		tab[i] = static_cast<float>(s.hasSheep());
		i++;
		tab[i] = static_cast<float>(s.hasWolf());
		i++;
		tab[i] = static_cast<float>(s.odour() / Square::ODOUR_MAX);
		i++;
		return i;
	}

	std::size_t DecisionTree::initArray(float* tab, int val)
	{
		tab[0] = static_cast<float>(val) / static_cast<float>(Square::getIntmax());
		return 1;
	}

	std::size_t DecisionTree::initArray(float* tab, const ReturnValue& val) {
		unsigned int i = 0;
		while (i < val.size()) {
			tab[i] = val[i];
			++i;
		}
		return i;
	}
}


