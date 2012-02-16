
#include <fstream>

#include "EnvironnementGenetic.hpp"

boost::random::random_device EnvironnementGenetic::_randomD;
boost::random::mt19937 EnvironnementGenetic::_gen(EnvironnementGenetic::_randomD);

bool EnvironnementGenetic::operator==(const EnvironnementGenetic& rhs)
{
	return _sheepDecisionTree == rhs._sheepDecisionTree
		&& _wolfDecisionTree == rhs._wolfDecisionTree
		&& _sheepGen == rhs._sheepGen
		&& _wolfGen == rhs._wolfGen;
}

bool EnvironnementGenetic::DecisionTreeGen::operator==(const EnvironnementGenetic::DecisionTreeGen& rhs)
{
	return _layerNum == rhs._layerNum
		&& _layerSize == rhs._layerSize
		&& _learningRate == rhs._learningRate
		&& _decisionStep == rhs._decisionStep
		&& _trainStep == rhs._trainStep
		&& _activationHidden == rhs._activationHidden
		&& _activationOutput == rhs._activationOutput;
}

bool EnvironnementGenetic::EntityGen::operator==(const EnvironnementGenetic::EntityGen& rhs)
{
	return _superStep == rhs._superStep
		&& _goodStep == rhs._goodStep
		&& _neutralStep == rhs._neutralStep
		&& _badStep == rhs._badStep;
}

bool EnvironnementGenetic::operator!=(const EnvironnementGenetic& rhs)
{
	return !operator==(rhs);
}

bool EnvironnementGenetic::DecisionTreeGen::operator!=(const EnvironnementGenetic::DecisionTreeGen& rhs)
{
	return !operator==(rhs);
}

bool EnvironnementGenetic::EntityGen::operator!=(const EnvironnementGenetic::EntityGen& rhs)
{
	return !operator==(rhs);
}

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

EnvironnementGenetic EnvironnementGenetic::createBetter(const std::string& value)
{
	EnvironnementGenetic better_ret;
	EnvironnementGenetic ret;
	double perf;
	double better_perf = 0;

	std::ifstream stream(value.c_str());

	if (!stream.good())
	{
		std::cout << "error load" << std::endl;
	}

	while (stream.good())
	{
		stream >> ret;
		stream >> perf;
		if (!stream.fail() && perf > better_perf)
		{
			better_perf = perf;
			better_ret = ret;
		}
	}

	std::cout << better_ret << std::endl;

	return better_ret;
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

std::ostream& operator<<(std::ostream& in, const EnvironnementGenetic& adn)
{
	in << adn._sheepDecisionTree << " " << adn._wolfDecisionTree << " " << adn._sheepGen << " " << adn._wolfGen;
	return in;
}

std::ostream& operator<<(std::ostream& in, const EnvironnementGenetic::DecisionTreeGen& treegen)
{
	in << treegen._layerNum << " " << treegen._layerSize << " ";
	in << treegen._learningRate << " " << treegen._decisionStep << " ";
	in << treegen._trainStep << " " << treegen._activationHidden << " " << treegen._activationOutput;
	return in;
}

std::ostream& operator<<(std::ostream& in, const EnvironnementGenetic::EntityGen& entitygen)
{
	in << entitygen._superStep << " " << entitygen._goodStep << " " << entitygen._neutralStep << " " << entitygen._badStep;
	return in;
}

std::ostream& operator<<(std::ostream& in, const FANN::activation_function_enum& activ)
{
	in << static_cast<int>(activ);
	return in;
}

std::istream& operator>>(std::istream& in, EnvironnementGenetic& adn)
{
	in >> adn._sheepDecisionTree >> adn._wolfDecisionTree >> adn._sheepGen >> adn._wolfGen;
	return in;
}

std::istream& operator>>(std::istream& in, EnvironnementGenetic::DecisionTreeGen& treegen)
{
	in >> treegen._layerNum >> treegen._layerSize >> treegen._learningRate >> treegen._decisionStep;
	in >> treegen._trainStep >> treegen._activationHidden >> treegen._activationOutput;
	return in;
}

std::istream& operator>>(std::istream& in, EnvironnementGenetic::EntityGen& entitygen)
{
	in >> entitygen._superStep >> entitygen._goodStep >> entitygen._neutralStep >> entitygen._badStep;
	return in;
}

std::istream& operator>>(std::istream& in, FANN::activation_function_enum& activ)
{
	int tmp = 0;

	in >> tmp;
	activ = static_cast<FANN::activation_function_enum>(tmp);
	return in;
}




