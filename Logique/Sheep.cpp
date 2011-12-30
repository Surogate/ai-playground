
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

	Sheep::~Sheep()
	{
		std::cout << "@ send xp sheep death" <<  std::endl;
		while (_actionStack.size()) {
			ActionStore& top = _actionStack.top();
			_tree.trainNot(top);
			_actionStack.pop();
		}
		std::cout << "@ send xp sheep death" <<  std::endl;
	}

	void Sheep::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, boost::bind(&Entity::eat, shared_from_this(), boost::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, boost::bind(&Entity::reproduce, shared_from_this(), boost::ref(board)));
	}

	EntityAction Sheep::computeAction() {
		_actionStack.push(ActionStore(_foodCount, _lastCompute, _getSquare(_loc), _getSquare(_loc + Coord::UP), _getSquare(_loc + Coord::LEFT), _getSquare(_loc + Coord::DOWN), _getSquare(_loc + Coord::RIGHT), 
			_getSquare(_loc + Coord::LEFT + Coord::UP), _getSquare(_loc + Coord::UP + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::LEFT), 
			_getSquare(_loc + Coord::UP + Coord::UP), _getSquare(_loc + Coord::RIGHT + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::DOWN), _getSquare(_loc + Coord::LEFT + Coord::LEFT)));
		DecisionTree::ReturnValue result = _tree.computeAction(_actionStack.top());
		_lastCompute = result;
		_actionStack.top().result = result;
		_actual++;
		return _tree.getValue(result);
	}

	void Sheep::sendXp() {
		while (_actionStack.size()) {
					ActionStore& top = _actionStack.top();
					_tree.train(top);		
					_actionStack.pop();
		}

		_tree.generateTree();
	}

	void Sheep::initExp() {
		//Square present;
		//present.hasGrass(true);
		//Square other;
		//Square other2;
		//other.hasGrass(true);

		//_tree.train(1, _tree.randomAction(), present, other, other, other, other, EAT);
		//_tree.train(5, _tree.randomAction(), present, other, other, other, other, EAT);
		//_tree.train(6, _tree.randomAction(), present, other, other, other, other, EAT);
		//other.hasGrass(false);
		//_tree.train(2, _tree.randomAction(), present, other, other, other, other, EAT);
		//_tree.train(3, _tree.randomAction(), present, other, other, other, other, EAT);
		//_tree.train(4, _tree.randomAction(), present, other, other, other, other, EAT);

		//other.hasSheep(reinterpret_cast<Entity*>(1));

		//_tree.train(7, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);
		//_tree.train(8, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);
		//_tree.train(9, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);

		//other2.hasSheep(reinterpret_cast<Entity*>(1));

		//_tree.train(7, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);
		//_tree.train(8, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);
		//_tree.train(9, _tree.randomAction(), present, other, other, other2, other2, REPRODUCE);
		//
		//_tree.generateTree();
	}

	Action Sheep::getNewAction() {
		EntityAction act = computeAction();

		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();
			
			if (_validScore(moy)) {
				sendXp();
				_lastMoy = moy;
				_tree.sendMoy(moy);
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
		if (isAlive() && _foodCount > _rep_limit && hasSheepNext() && _popEntity(_loc)) {
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

