
#ifndef ACTIONSTORE
#define ACTIONSTORE

#include <boost/function.hpp>

#include "DecisionTree.hpp"
#include "Coord.hpp"
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

		ActionStore(unsigned int _foodcount_,  DecisionTree::ReturnValue _last_, const Coord& start, boost::function< Square& (const Coord&) >& getSquare)
			: foodcount(_foodcount_), 
			present(getSquare(start)), up(getSquare(start + Coord::UP)), left(getSquare(start + Coord::LEFT)), right(getSquare(start + Coord::RIGHT)),
			upleft(getSquare(start + Coord::UP + Coord::LEFT)), upright(getSquare(start + Coord::UP + Coord::RIGHT)), downright(getSquare(start + Coord::DOWN + Coord::RIGHT)), downleft(getSquare(start + Coord::DOWN + Coord::LEFT)),
			upup(getSquare(start + Coord::UP + Coord::UP)), rightright(getSquare(start + Coord::RIGHT + Coord::RIGHT)), downdown(getSquare(start + Coord::DOWN + Coord::DOWN)), leftleft(getSquare(start + Coord::LEFT + Coord::LEFT)),
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