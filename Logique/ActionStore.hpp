
#ifndef ACTIONSTORE
#define ACTIONSTORE

#include <boost/function.hpp>
#include <vector>

#include "DecisionTree.hpp"
#include "Coord.hpp"
#include "Square.hpp"
#include "FixedVector.hpp"

namespace Logique {

	struct ActionStore 
	{
		enum {
			VISION_SIZE = 5
		};

		unsigned int foodcount;
		Square present;
		Square up;
		Square left;
		Square down;
		Square right;
		FixedVector< int, 128 > _case;

		DecisionTree::ReturnValue result;
		DecisionTree::ReturnValue last;

		ActionStore(unsigned int _foodcount_,  DecisionTree::ReturnValue _last_, const Coord& start, boost::function< Square& (const Coord&) >& getSquare);
		void getLine(Coord pos, int diff, boost::function< Square& (const Coord&) >& getSquare);
		
		std::size_t initArray(float* _input) const;
		std::size_t initArray(float* tab, const Square& s) const;
		std::size_t initArray(float* tab, int val) const;
		std::size_t initArray(float* tab, const DecisionTree::ReturnValue& val) const;

		static std::size_t getSize();
	};

}

#endif