
#ifndef ENVIRONNEMENTGENETIC_HPP
#define ENVIRONNEMENTGENETIC_HPP

#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include "fann.h"
#include "fann_cpp.h"

struct EnvironnementGenetic
{
	enum {
		LAYERNUMMIN = 1,
		LAYERNUMMAX = 7,
		LAYERSIZEMIN = 20,
		LAYERSIZEMAX = 300
	};

	struct DecisionTreeGen {
		unsigned int _layerNum;
		unsigned int _layerSize;
		float _learningRate;
		float _decisionStep;
		float _trainStep;
		FANN::activation_function_enum _activationHidden;
		FANN::activation_function_enum _activationOutput;
	} _sheepDecisionTree, _wolfDecisionTree;

	struct EntityGen {
		float _superStep;
		float _goodStep;
		float _neutralStep;
		float _badStep;
	} _sheepGen, _wolfGen;

	static EnvironnementGenetic randomGen();
	static EnvironnementGenetic reproduce(const EnvironnementGenetic& lhs, const EnvironnementGenetic& rhs);
private:
	static boost::random::random_device _randomD;
	static boost::random::mt19937 _gen;

	static DecisionTreeGen randomTree();
	static EntityGen randomEntity();
	static DecisionTreeGen reproduce(const DecisionTreeGen& lhs, const DecisionTreeGen& rhs);
	static EntityGen reproduce(const EntityGen& lhs, const EntityGen& rhs);
	
	static inline unsigned int randomBetween(unsigned int lhs, unsigned int rhs)
	{
		if (lhs != rhs) 
		{
			if (lhs < rhs)
			{
				boost::random::uniform_int_distribution<unsigned int> dis(lhs, rhs);
				return dis(_gen);
			} 
			else 
			{
				boost::random::uniform_int_distribution<unsigned int> dis(rhs, lhs);
				return dis(_gen);
			}
		}
		return lhs;
	}

	static inline float randomBetween(float lhs, float rhs)
	{
		if (lhs != rhs) 
		{
			if (lhs < rhs)
			{
				boost::random::uniform_real_distribution<float> dis(lhs, rhs);
				return dis(_gen);
			} 
			else 
			{
				boost::random::uniform_real_distribution<float> dis(rhs, lhs);
				return dis(_gen);
			}
		}
		return lhs;
	}
};

#endif /* !ENVIRONNEMENTGENETIC_HPP */