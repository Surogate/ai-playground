
#include <iostream>
#include <cmath>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	Sheep::Sheep(DecisionTree& tree) : Entity(Square::SHEEP, tree)
	{}

	Sheep::~Sheep()
	{
		onDeath();
	}

	void Sheep::onDeath() 
	{
		sendXpNot(0.1f);
	}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		
		_actionArray[EAT] = boost::bind(&Sheep::eat, this, boost::ref(board));
		_timeArray[EAT] = EAT_TIME;
		_actionArray[REPRODUCE] = boost::bind(&Sheep::reproduce, this, boost::ref(board));
		_timeArray[REPRODUCE] = REPRODUCE_TIME;
	}

	void Sheep::genXp() {
		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (moy > 0) {
				_lastMoy = moy;
				if (_validScore(moy)) {
					sendXp(0.25f);
				} else {
					sendXp(0.2f);
				}
			} else {
				sendXpNot(0.05f);
			}
			reInitPerf();
		}
	}

	void Sheep::eat(Board& board) {
		if (board(_loc).hasGrass()) {
			_lastAction = EAT;
			_numberEat++;
			board.lock();
			board(_loc).hasGrass(false);
			board.unlock();
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
			Callback_Environnement::getInstance().addAction(Environnement_Event::GRASS_DOWN, _loc);
		}
	}

	void Sheep::reproduce(Board& board) {
		if (_rep_limit > REPRODUCE_COUNTER && hasSheepNext() && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep++;
			_rep_limit = REPRODUCE_COUNTER / 2;
			if (_numberRep < 2)
				_rep_limit += 2;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);
		}
	}

	bool Sheep::hasSheepNext() {
		int up = _getSquare(_loc + Coord::UP);
		int left = _getSquare(_loc + Coord::LEFT);
		int down = _getSquare(_loc + Coord::DOWN);
		int right = _getSquare(_loc + Coord::RIGHT);
		return (up & Square::SHEEP_MASK) 
			|| (left & Square::SHEEP_MASK)
			|| (down & Square::SHEEP_MASK)
			|| (right & Square::SHEEP_MASK);
	}
}

