
#ifndef ACTIONSTORE
#define ACTIONSTORE

#include "DecisionTree.hpp"
#include "Square.hpp"

namespace Logique {

	struct ActionStore 
	{
		unsigned int foodcount;
		Square present;
		Square up;
		Square left;
		Square down;
		Square right;
		int upleft;
		int upright;
		int downright;
		int downleft;
		int upup;
		int rightright;
		int downdown;
		int leftleft;

		DecisionTree::ReturnValue result;
		DecisionTree::ReturnValue last;

		ActionStore(unsigned int _foodcount_, const Square& _present_, const Square& _up_, const Square& _left_, const Square& _down_, const Square& _right_, DecisionTree::ReturnValue _result_, DecisionTree::ReturnValue _last_)
			: foodcount(_foodcount_), present (_present_), up(_up_), left(_left_), down(_down_), right(_right_), result(_result_), last(_last_)
		{}

		ActionStore(unsigned int _foodcount_, DecisionTree::ReturnValue _last_, 
			const Square& _present_, const Square& _up_, const Square& _left_, const Square& _down_, const Square& _right_,
			int _upleft_, int _upright_, int _downright_, int _downleft_, int _upup_, int _rightright_, int _downdown_, int _leftleft_, 
			DecisionTree::ReturnValue _result_) 
			: foodcount(_foodcount_), 
			present(_present_), up(_up_), left(_left_), down(_down_), right(_right_), 
			upleft(_upleft_), upright(_upright_), downright(_downright_), downleft(_downleft_), upup(_upup_), rightright(_rightright_), downdown(_downdown_), leftleft(_leftleft_),
			result(_result_), last(_last_)
		{
		}

		ActionStore(unsigned int _foodcount_, DecisionTree::ReturnValue _last_, 
			const Square& _present_, const Square& _up_, const Square& _left_, const Square& _down_, const Square& _right_,
			int _upleft_, int _upright_, int _downright_, int _downleft_, int _upup_, int _rightright_, int _downdown_, int _leftleft_) 
			: foodcount(_foodcount_), 
			present(_present_), up(_up_), left(_left_), down(_down_), right(_right_), 
			upleft(_upleft_), upright(_upright_), downright(_downright_), downleft(_downleft_), upup(_upup_), rightright(_rightright_), downdown(_downdown_), leftleft(_leftleft_),
			last(_last_)
		{
		}

		std::size_t initArray(float* _input) const {
			std::size_t i = 0;
			_input[i] = static_cast<float>(foodcount / (Logique::FOOD_MAX));
			i++;
			i += initArray(&_input[i], last);
			i += initArray(&_input[i], present);
			i += initArray(&_input[i], up);
			i += initArray(&_input[i], left);
			i += initArray(&_input[i], down);
			i += initArray(&_input[i], right);
			i += initArray(&_input[i], upleft);
			i += initArray(&_input[i], upright);
			i += initArray(&_input[i], downright);
			i += initArray(&_input[i], downleft);
			i += initArray(&_input[i], upup);
			i += initArray(&_input[i], rightright);
			i += initArray(&_input[i], downdown);
			i += initArray(&_input[i], leftleft);

			return i;
		}

		std::size_t initArray(float* tab, const Square& s) const
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

		std::size_t initArray(float* tab, int val) const
		{
			tab[0] = static_cast<float>(val) / static_cast<float>(Square::getIntmax());
			return 1;
		}

		std::size_t initArray(float* tab, const DecisionTree::ReturnValue& val) const {
			unsigned int i = 0;
			while (i < val.size()) {
				tab[i] = val[i];
				++i;
			}
			return i;
		}
	};

}

#endif