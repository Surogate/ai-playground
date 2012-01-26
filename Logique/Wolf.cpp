
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {
	Wolf::Wolf(DecisionTree& tree) 
		: Entity(Square::WOLF, tree)
	{}

	Wolf::~Wolf() 
	{
		onDeath();
	}

	void Wolf::onDeath() 
	{
		sendXpNot(0.1f);
	}

	void Wolf::initActionArray(Board& board) 
	{
		Entity::initActionArray(board);

		_actionArray[EAT] = boost::bind(&Wolf::eat, this, boost::ref(board));
		_timeArray[EAT] = EAT_TIME;
		_actionArray[REPRODUCE] = boost::bind(&Wolf::reproduce, this, boost::ref(board));
		_timeArray[REPRODUCE] = REPRODUCE_TIME;
	}

	void Wolf::genXp() {
		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (moy > 0) {
				_lastMoy = moy;
				if (_validScore(moy)) {
					sendXp(0.4f);
				} else {
					sendXp(0.3f);
				}
			} else {
				sendXpNot(0.03f);
			}
			reInitPerf();
		}
	}

	void Wolf::eat(Board& board) 
	{
		if (isAlive() && _getSquare(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			Entity* sheep =_getSquare(_loc).getEntity(Square::SHEEP);
			if (sheep) {
				sheep->setFood(0);
			}
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
		}
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive() && _rep_limit > REPRODUCE_COUNTER && hasWolfNext() && _popEntity(_loc) && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep += 2;
			_rep_limit = REPRODUCE_COUNTER / 2;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);			
		}
	}

	bool Wolf::hasWolfNext() {
		int up = _getSquare(_loc + Coord::UP);
		int left = _getSquare(_loc + Coord::LEFT);
		int down = _getSquare(_loc + Coord::DOWN);
		int right = _getSquare(_loc + Coord::RIGHT);
		return (up & Square::WOLF_MASK) 
			|| (left & Square::WOLF_MASK)
			|| (down & Square::WOLF_MASK)
			|| (right & Square::WOLF_MASK);
	}
}
