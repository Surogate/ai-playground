
#ifndef ENVIRONNEMENTGENETIC_HPP
#define ENVIRONNEMENTGENETIC_HPP

#include "fann_cpp.h"

struct EnvironnementGenetic
{
	struct DecisionTreeGen {
		unsigned int _layerNum;
		unsigned int _layerSize;
		float _learningRate;
		float _decisionStep;
		float _trainStep;
		const FANN::activation_function_enum _activation;
	} _sheepDecisionTree, _wolfDecisionTree;

	struct EntityGen {
		float _superStep;
		float _goodStep;
		float _neutralStep;
		float _badStep;
	} _sheepGen, _wolfGen;
};

#endif /* !ENVIRONNEMENTGENETIC_HPP */