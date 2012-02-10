
#include <iostream>
#include <cmath>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Environnement.hpp"
#include "Logger.hpp"

namespace Logique {
	Sheep::Sheep(DecisionTree& tree) 
		: Entity(Square::SHEEP, tree)
	{}

	Sheep::Sheep(DecisionTree& tree, const EnvironnementGenetic::EntityGen& adn) 
		: Entity(Square::SHEEP, tree, adn)
	{}

	Sheep::~Sheep()
	{ 
		_actionStack.clear();
		onDeath(); 
	}

	void Sheep::onDeath() 
	{ sendXpNot(BADSTEP); }

	void Sheep::initActionArray(Board& board, Environnement& env) 
	{
		Entity::initActionArray(board, env);
		
		_actionArray[EAT] = ActionPair(EAT_TIME, boost::bind(&Sheep::eat, this, boost::ref(board), boost::ref(env)));
		_actionArray[REPRODUCE] = ActionPair(REPRODUCE_TIME, boost::bind(&Sheep::reproduce, this, boost::ref(board), boost::ref(env)));
	}

	void Sheep::genXp() 
	{
		if (_actual && _actual >= _numberTot) 
		{
			float moy = computeMoy();

			if (moy > 0) 
			{
				_lastMoy = moy;
				if (_validScore(moy)) 
				{ sendXp(SUPERSTEP); } 
				else 
				{ sendXp(GOODSTEP); }
			}
			else 
			{ sendXpNot(NEUTRALSTEP); }
			reInitPerf();
		}
	}

	void Sheep::destroyMe(Environnement& env)
	{
		env.destroySheep(this);
	}

	void Sheep::eat(Board& board, Environnement& env) 
	{
		if (board(_loc).hasGrass()) 
		{
			_lastAction = EAT;
			_numberEat++;
			board.lock();
			board(_loc).decreaseGrass(GRASS_EATED);
			board.unlock();
			addFood(FOOD_GAIN);
			env.addEvent(Environnement_Event::ENTITY_EAT, *this, _type, _loc);
			if (!board(_loc).hasGrass())
			{ env.addEvent(Environnement_Event::GRASS_DOWN, _loc); }
		}
	}

	void Sheep::reproduce(Board& board, Environnement& env) 
	{
		if (_rep_limit > REPRODUCE_COUNTER && hasSheepNext() && _popEntity(_loc) && _popEntity(_loc)) 
		{
			_numberRep++;
			_rep_limit = REPRODUCE_COUNTER / 2;
			if (_numberRep < 2)
			{ _rep_limit += 2; }
			_lastAction = REPRODUCE;
			env.addEvent(Environnement_Event::ENTITY_REPRODUCE, *this, _type, _loc);
		}
	}

	bool Sheep::hasSheepNext() 
	{
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

