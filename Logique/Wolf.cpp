
#include "Wolf.hpp"
#include "Board.hpp"
#include "Environnement.hpp"
#include "Logger.hpp"

namespace Logique {
	Wolf::Wolf(DecisionTree& tree) 
		: Entity(Square::WOLF, tree)
	{
		SUPERSTEP = 0.4f;
		GOODSTEP = 0.3f;
		NEUTRALSTEP = 0.03f;
		BADSTEP = 0.1f;
	}

	Wolf::Wolf(DecisionTree& tree, const EnvironnementGenetic::EntityGen& adn) 
		: Entity(Square::WOLF, tree, adn)
	{}

	Wolf::~Wolf() 
	{
		onDeath();
	}

	void Wolf::onDeath() 
	{
		sendXpNot(BADSTEP);
	}

	void Wolf::initActionArray(Board& board, Environnement& env) 
	{
		Entity::initActionArray(board, env);

		_actionArray[EAT] = ActionPair(EAT_TIME, boost::bind(&Wolf::eat, this, boost::ref(board), boost::ref(env)));
		_actionArray[REPRODUCE] = ActionPair(REPRODUCE_TIME, boost::bind(&Wolf::reproduce, this, boost::ref(board), boost::ref(env)));
	}

	void Wolf::genXp() {
		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (moy > 0) {
				_lastMoy = moy;
				if (_validScore(moy)) {
					sendXp(SUPERSTEP);
				} else {
					sendXp(GOODSTEP);
				}
			} else {
				sendXpNot(NEUTRALSTEP);
			}
			reInitPerf();
		}
	}

	void Wolf::destroyMe(Environnement& env)
	{
		env.destroyWolf(this);
	}

	void Wolf::eat(Board& board, Environnement& env) 
	{
		if (isAlive() && _getSquare(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			Entity* sheep =_getSquare(_loc).getEntity(Square::SHEEP);
			if (sheep) {
				sheep->setFood(0);
			}
			addFood(FOOD_GAIN);
			env.addEvent(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
		}
	}

	void Wolf::reproduce(Board& board, Environnement& env) {
		if (isAlive() && _rep_limit > REPRODUCE_COUNTER && hasWolfNext() && _popEntity(_loc) && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep += 2;
			_rep_limit = REPRODUCE_COUNTER / 2;
			_lastAction = REPRODUCE;
			env.addEvent(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);			
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
