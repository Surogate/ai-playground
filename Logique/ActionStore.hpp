
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

		DecisionTree::ReturnValue result;
		DecisionTree::ReturnValue last;

		ActionStore(unsigned int foodcount, const Square& _present_, const Square& _up_, const Square& _left_, const Square& _down_, const Square& _right_, DecisionTree::ReturnValue _result_, DecisionTree::ReturnValue _last_)
			: present (_present_), up(_up_), left(_left_), down(_down_), right(_right_), result(_result_), last(_last_)
		{}
	};

}

#endif