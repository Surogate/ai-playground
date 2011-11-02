
#include <iostream>
#include <cmath>

#include "Sheep.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Sheep::_tree;

	Sheep::Sheep() : Entity(Square::SHEEP)
	{}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, boost::bind(&Entity::eat, shared_from_this(), boost::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, boost::bind(&Entity::reproduce, shared_from_this(), boost::ref(board)));
	}

	Entity::EntityAction Sheep::computeAction() {
		int up = getIntFromLess(_loc, Coord::DOWN);
		int left = getIntFromLess(_loc, Coord::RIGHT);
		int down = getIntFromSup(_loc, Coord::DOWN);
		int right = getIntFromSup(_loc, Coord::RIGHT);
		EntityAction act = _tree.computeAction(_getSquare(_loc), up, left, down, right); 
		_actionStack.push(ActionStore(_getSquare(_loc), up, left, down, right, act));
		_actual++;
		return act;
	}

	Action Sheep::getNewAction() {
		EntityAction act = computeAction();

		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();
			
			if (_validScore(moy)) {
				while (_actionStack.size()) {
					ActionStore& top = _actionStack.top();
					_tree.addAction(top.present, top.up, top.left, top.down, top.right, top.result);
					_actionStack.pop();
				}
				Logger log("Mouton.log");
				log.dump(moy);
				std::cout << "#Sheep action commited - old perf " << _lastMoy << std::endl;
				std::cout << "#Sheep new perf " << moy << std::endl;
				std::cout << "tree moy " << _tree.getMoy() << std::endl;
				std::cout << "#Sheep experience size " << _tree.getSize() << std::endl;
				_lastMoy = moy;
				_tree.sendMoy(moy);
				std::cout << "generate tree" << std::endl;
				_tree.generateTree();
				std::cout << "tree generated" << std::endl;
			}
			reInitPerf();
		}
		return _actionArray[act];
	}

	void Sheep::eat(Board& board) {
		if (isAlive() && board(_loc).hasGrass()) {
			_lastAction = EAT;
			_numberEat++;
			std::cout << "sheep eat" << std::endl;
			board.lock();
			board(_loc).hasGrass(false);
			board.unlock();
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Sheep::reproduce(Board& board) {
		if (isAlive() && _foodCount >= FOOD_REP_LIMIT && hasSheepNext() && _popEntity(_loc)) {
			_numberRep++;
			std::cout << "sheep reproduce" << std::endl;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().cb_onEntityReproduce(*this);
		}
		generateNewAction();
	}

	bool Sheep::hasSheepNext() {
		int up = getIntFromLess(_loc, Coord::DOWN);
		int left = getIntFromLess(_loc, Coord::RIGHT);
		int down = getIntFromSup(_loc, Coord::DOWN);
		int right = getIntFromSup(_loc, Coord::RIGHT);
		return (up & Square::SHEEP_MASK) 
			|| (left & Square::SHEEP_MASK)
			|| (down & Square::SHEEP_MASK)
			|| (right & Square::SHEEP_MASK);
	}
}

