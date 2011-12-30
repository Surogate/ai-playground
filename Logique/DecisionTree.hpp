
#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/array.hpp>
#include "ID3Class.hpp"
#include "fann.h"
#include "fann_cpp.h"
#include "Square.hpp"
#include "EntityConstant.hpp"

namespace Logique {

	class DecisionTree {
	public:
		enum {
			INPUTFOODSIZE = 1,
			INPUTLASTACTIONSIZE = ACTION_CONTAINER_SIZE,
			SQUAREEXPANDSIZE = 5,
			SQUAREINTSIZE = 1,
			INPUTSIZE = INPUTFOODSIZE + INPUTLASTACTIONSIZE + 5 * SQUAREEXPANDSIZE + 8 * SQUAREINTSIZE,
			OUTPUTSIZE = ACTION_CONTAINER_SIZE,
			HIDDENSIZE = INPUTSIZE,
			LAYERNUM = 3
		};

		static const float DECISIONSTEP;

		typedef boost::array<float, OUTPUTSIZE> OutputArray;
		typedef boost::array<float, INPUTSIZE> InputArray;
		typedef OutputArray ReturnValue;

		DecisionTree();

		ReturnValue computeAction(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite);
		ReturnValue computeAction(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right,
			int upleft, int upright, int downright, int downleft, int upup, int rightright, int downdown, int leftleft);

		void train(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, ReturnValue result);
		
		void train(unsigned int foodcount, ReturnValue lastAction, 
			const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite,
			int upleft, int upright, int downright, int downleft, int upup, int rightright, int downdown, int leftleft,
			ReturnValue result);
		
		void trainNot(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite, ReturnValue result);
		void trainNot(unsigned int foodcount, ReturnValue lastAction, 
			const Square& present, const Square& haut, const Square& gauche, const Square& bas, const Square& droite,
			int upleft, int upright, int downright, int downleft, int upup, int rightright, int downdown, int leftleft,
			ReturnValue result);
		const float& getMoy() const;
		void sendMoy(float value);
		void generateTree();
		EntityAction randomAction();
		EntityAction getValue(const ReturnValue& ret);

	private:
		void initArray(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right);
		void initArray(unsigned int foodcount, ReturnValue lastAction, const Square& present, const Square& up, const Square& left, const Square& down, const Square& right,
			int upleft, int upright, int downright, int downleft, int upup, int rightright, int downdown, int leftleft);

		std::size_t initArray(float* tab, const Square& s);
		std::size_t initArray(float* tab, int val);
		std::size_t initArray(float* tab, const ReturnValue& val);

		FANN::neural_net _ann;
		OutputArray _output;
		InputArray _input;

		float _moyenne;
		boost::random::random_device _randomD;
		boost::random::mt19937 _gen;
		boost::random::uniform_int_distribution<unsigned int> _distri;
	};

}

#endif /* !DECISIONTREE_HPP */