
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
		FixedVector< int, 128 > _case;

		DecisionTree::ReturnValue result;

		ActionStore(unsigned int _foodcount_, const Coord& start, boost::function< Square& (const Coord&) >& getSquare);
		void getLine(Coord pos, int diff, boost::function< Square& (const Coord&) >& getSquare);
		
		std::size_t initArray(float* input) const;
		std::size_t initArray(float* tab, const Square& s) const;
		std::size_t initArray(float* tab, int val) const;
		std::size_t initArray(float* tab, const DecisionTree::ReturnValue& val) const;

		static std::size_t getSize();
	};

}

#endif