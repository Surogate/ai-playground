
#include "ActionStore.hpp"

namespace Logique {

	ActionStore::ActionStore(unsigned int _foodcount_,  DecisionTree::ReturnValue _last_, const Coord& start, boost::function< Square& (const Coord&) >& getSquare)
		: foodcount(_foodcount_), 
		present(getSquare(start)), up(getSquare(start + Coord::UP)), left(getSquare(start + Coord::LEFT)), right(getSquare(start + Coord::RIGHT)),
		last(_last_)
	{
		_case.reserve(getSize());
			
		Coord calc_start = start;
			
		for (unsigned int i = 0; i < VISION_SIZE - 1; ++i) {
			calc_start += Coord::LEFT;
		}

		_case.push_back(getSquare(calc_start));
		for (unsigned int i = 0; i < (VISION_SIZE - 1) * 2; ++i) {
			calc_start += Coord::RIGHT;
			int diff;
			if (i < VISION_SIZE -1)
				diff = (VISION_SIZE - 1) - (start.y - calc_start.y);
			else
				diff = (VISION_SIZE - 1) + (start.y - calc_start.y);
			getLine(calc_start, diff, getSquare);
		}
	}

	void ActionStore::getLine(Coord pos, int diff, boost::function< Square& (const Coord&) >& getSquare) {
		for (int i = 0; i < diff; ++i) {
			pos += Coord::UP;
		}
		for (int i = (-1 * diff); i < diff + 1; ++i) {
			_case.push_back(getSquare(pos));
			pos += Coord::DOWN;
		}
	}

	std::size_t ActionStore::getSize() {
		std::size_t size_max = (VISION_SIZE - 1) * 2 + 1;
		std::size_t size_total = size_max;
		while (size_max > 1) {
			size_max -= 2;
			size_total += size_max * 2;
		}

		return size_total;
	}

	std::size_t ActionStore::initArray(float* _input) const 
	{
		std::size_t i = 0;
		_input[i] = static_cast<float>(foodcount / (Logique::FOOD_MAX));
		i++;
		i += initArray(&_input[i], last);
		i += initArray(&_input[i], present);
		i += initArray(&_input[i], up);
		i += initArray(&_input[i], left);
		i += initArray(&_input[i], down);
		i += initArray(&_input[i], right);
		for (unsigned int o = 0; o < _case.size(); ++o) {
			i += initArray(&_input[i], _case[o]);
		}

		return i;
	}

	std::size_t ActionStore::initArray(float* tab, const Square& s) const
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

	std::size_t ActionStore::initArray(float* tab, int val) const
	{
		tab[0] = static_cast<float>(val) / static_cast<float>(Square::getIntmax());
		return 1;
	}

	std::size_t ActionStore::initArray(float* tab, const DecisionTree::ReturnValue& val) const {
		unsigned int i = 0;
		while (i < val.size()) {
			tab[i] = val[i];
			++i;
		}
		return i;
	}

}
