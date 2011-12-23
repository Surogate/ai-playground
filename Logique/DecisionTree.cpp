
#include "DecisionTree.hpp"

namespace Logique {
	DecisionTree::DecisionTree() 
		: _ann(), _moyenne(0.f)
		, _randomD(), _gen(_randomD), _distri(0, Entity::ACTION_CONTAINER_SIZE - 1)
	{
		_ann.create_standard(3, 27, 27, Entity::ACTION_CONTAINER_SIZE);
		_ann.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		_ann.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		_ann.randomize_weights(0.3f, 0.7f);
	}

	Entity::EntityAction DecisionTree::randomAction() 
	{
		return static_cast<Entity::EntityAction>(_distri(_gen));
	}

	Entity::EntityAction DecisionTree::computeAction(unsigned int foodcount, Entity::EntityAction lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		float* output = _ann.run(_input.c_array());
		std::size_t best = 0;
		float val = output[0];
		std::size_t i = 0;
		bool notClearAnswer = true;
		while (i < Entity::ACTION_CONTAINER_SIZE)
		{
			if (output[i] >= val)
			{
				if (output[i] - val > 0.1f)
					notClearAnswer = false;
				best = i;
				val = output[i];
			}
			++i;
		}
		if (notClearAnswer)
			return randomAction();
		return static_cast<Entity::EntityAction>(best);
	}

	const float& DecisionTree::getMoy() const 
	{
		return _moyenne;
	}

	void DecisionTree::train(unsigned int foodcount, Entity::EntityAction lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, Entity::EntityAction result) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		initOutputArray(result, 0, 1);
		_ann.train(_input.c_array(), _output.c_array());
	}

	void DecisionTree::trainNot(unsigned int foodcount, Entity::EntityAction lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, Entity::EntityAction result) 
	{
		initInputArray(foodcount, lastAction, present, haut, gauche, bas, droite);
		initOutputArray(result, 1, 0);
		_ann.train(_input.c_array(), _output.c_array());
	}

	void DecisionTree::sendMoy(float value) 
	{
		_moyenne = (_moyenne + value) / 2.f;
	}

	void DecisionTree::generateTree() 
	{
	}

	void DecisionTree::initInputArray(unsigned int foodcount, Entity::EntityAction lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right)
	{
		std::size_t i = 0;
		_input[i] = static_cast<float>(foodcount / (Entity::FOOD_MAX));
		i++;
		_input[i] = static_cast<float>(lastAction / (Entity::ACTION_CONTAINER_SIZE - 1));
		i++;
		i += initInputArray(&_input[i], present);
		i += initInputArray(&_input[i], up);
		i += initInputArray(&_input[i], left);
		i += initInputArray(&_input[i], down);
		i += initInputArray(&_input[i], right);
	}

	void DecisionTree::initOutputArray(Entity::EntityAction val, float valBase, float valChoice)
	{
		for (std::size_t i = 0; i < Entity::ACTION_CONTAINER_SIZE; ++i) {
			_output[i] = valBase;
		}
		_output[val] = valChoice;
	}

	std::size_t DecisionTree::initInputArray(float* tab, const Square& s)
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

	std::size_t DecisionTree::initInputArray(float* tab, int val)
	{
		tab[0] = static_cast<float>(val) / static_cast<float>(Square::getIntmax());
		return 1;
	}

}


