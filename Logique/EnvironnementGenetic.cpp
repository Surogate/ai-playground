
#include "EnvironnementGenetic.hpp"

boost::random::random_device EnvironnementGenetic::_randomD;
boost::random::mt19937 EnvironnementGenetic::_gen(EnvironnementGenetic::_randomD);

EnvironnementGenetic EnvironnementGenetic::randomGen()
{
	EnvironnementGenetic result;

	result._sheepDecisionTree = randomTree();
	result._wolfDecisionTree = randomTree();
	result._sheepGen = randomEntity();
	result._wolfGen = randomEntity();
	return result;
}

EnvironnementGenetic EnvironnementGenetic::reproduce(const EnvironnementGenetic& lhs, const EnvironnementGenetic& rhs)
{
	EnvironnementGenetic result;
	
	result._sheepDecisionTree = reproduce(lhs._sheepDecisionTree, rhs._sheepDecisionTree);
	result._wolfDecisionTree = reproduce(lhs._wolfDecisionTree, rhs._wolfDecisionTree);
	result._sheepGen = reproduce(lhs._sheepGen, rhs._sheepGen);
	result._wolfGen = reproduce(lhs._wolfGen, rhs._wolfGen);

	return result;
}

EnvironnementGenetic::DecisionTreeGen EnvironnementGenetic::randomTree()
{
	DecisionTreeGen result;

	boost::random::uniform_int_distribution<unsigned int> layerNum(LAYERNUMMIN, LAYERNUMMAX);
	boost::random::uniform_int_distribution<unsigned int> layerSize(LAYERSIZEMIN, LAYERSIZEMAX);
	boost::random::uniform_real_distribution<float> learningRate(0.05f, 0.9f);
	boost::random::uniform_real_distribution<float> decisionStep(0.0005f, 0.9f);
	boost::random::uniform_real_distribution<float> trainStep(0.0005f, 0.9f);
	
	FANN::activation_function_enum tab[] = {
        FANN::SIGMOID,
        FANN::SIGMOID_STEPWISE,
        FANN::GAUSSIAN,
        FANN::GAUSSIAN_STEPWISE
	};

	boost::random::uniform_int_distribution<unsigned int> activation(0, sizeof(tab) / sizeof(int) - 1);
	
	result._layerNum = layerNum(_gen);
	result._layerSize = layerSize(_gen);;
	result._learningRate = learningRate(_gen);
	result._decisionStep = decisionStep(_gen);
	result._trainStep = trainStep(_gen);
	result._activationHidden = tab[activation(_gen)];
	result._activationOutput = tab[activation(_gen)];

	return result;
}

EnvironnementGenetic::EntityGen EnvironnementGenetic::randomEntity()
{
	EntityGen result;

	boost::random::uniform_real_distribution<float> step(0.f, 1.f);

	result._superStep = step(_gen);
	result._neutralStep = step(_gen);
	result._goodStep = step(_gen);
	result._badStep = step(_gen);

	return result;
}

EnvironnementGenetic::DecisionTreeGen EnvironnementGenetic::reproduce(const DecisionTreeGen& lhs, const DecisionTreeGen& rhs)
{
	DecisionTreeGen result;

	boost::random::uniform_int_distribution<unsigned int> activation(0, 1);
	
	if (activation(_gen) == 0)
		result._activationHidden = lhs._activationHidden;
	else
		result._activationHidden = rhs._activationHidden;

	if (activation(_gen) == 0)
		result._activationOutput = lhs._activationOutput;
	else
		result._activationOutput = rhs._activationOutput;

	result._layerNum = randomBetween(lhs._layerNum, rhs._layerNum);
	result._layerSize = randomBetween(lhs._layerSize, rhs._layerSize);
	result._learningRate = randomBetween(lhs._learningRate, rhs._learningRate);
	result._decisionStep = randomBetween(lhs._decisionStep, rhs._decisionStep);
	result._trainStep = randomBetween(lhs._trainStep, rhs._trainStep);

	return result;
}

EnvironnementGenetic::EntityGen EnvironnementGenetic::reproduce(const EntityGen& lhs, const EntityGen& rhs)
{
	EntityGen result;

	result._badStep = randomBetween(lhs._badStep, rhs._badStep);
	result._superStep = randomBetween(lhs._superStep, rhs._superStep);
	result._goodStep = randomBetween(lhs._goodStep, rhs._goodStep);
	result._neutralStep = randomBetween(lhs._neutralStep, rhs._neutralStep);

	return result;
}


