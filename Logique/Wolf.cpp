
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Wolf::_tree;

	Wolf::Wolf() 
		: Entity(Square::WOLF)
	{}

	Wolf::~Wolf() 
	{
		sendXpNot(1.f);
	}

	void Wolf::initActionArray(Board& board) 
	{
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, boost::bind(&Entity::eat, shared_from_this(), boost::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, boost::bind(&Entity::reproduce, shared_from_this(), boost::ref(board)));
	}

	EntityAction Wolf::computeAction() 
	{
		_actionStack.push(ActionStore(_foodCount, _lastCompute, _getSquare(_loc), _getSquare(_loc + Coord::UP), _getSquare(_loc + Coord::LEFT), _getSquare(_loc + Coord::DOWN), _getSquare(_loc + Coord::RIGHT), 
			_getSquare(_loc + Coord::LEFT + Coord::UP), _getSquare(_loc + Coord::UP + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::LEFT), 
			_getSquare(_loc + Coord::UP + Coord::UP), _getSquare(_loc + Coord::RIGHT + Coord::RIGHT), _getSquare(_loc + Coord::DOWN + Coord::DOWN), _getSquare(_loc + Coord::LEFT + Coord::LEFT)));
		DecisionTree::ReturnValue result = _tree.computeAction(_actionStack.top());
		_lastCompute = result;
		_actionStack.top().result = result;
		_actual++;
		return _tree.getValue(result);
	}

	void Wolf::initExp() 
	{
		//Square present;
		//present.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		//Square other;
		//other.hasGrass(true);

		//_tree.train(0, _tree.randomAction(), present, other, other, other, other, EAT);
		//other.hasGrass(false);
		//_tree.train(1, _tree.randomAction(), present, other, other, other, other, EAT);
		//other.hasSheep(0);
		//_tree.train(2, _tree.randomAction(), present, other, other, other, other, EAT);
		//other.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		//_tree.train(3, _tree.randomAction(), present, 0, other, other, other, EAT);
		//other.addOdour(1);
		//_tree.train(4, _tree.randomAction(), present, other, 0, other, other, EAT);
		//other.addOdour(1);
		//_tree.train(5, _tree.randomAction(), present, other, other, other, 0, EAT);
		//other.addOdour(1);
		//_tree.train(6, _tree.randomAction(), present, other, other, 0, other, EAT);
		//other.addOdour(1);
		//other.hasGrass(true);
		//_tree.train(7, _tree.randomAction(), present, 0, other, other, 0, EAT);
		//other.addOdour(1);
		//_tree.train(8, _tree.randomAction(), present, other, other, other, other, EAT);
		//other.addOdour(10);
		//_tree.train(9, _tree.randomAction(), present, other, 0, 0, other, EAT);

		//other.hasWolf(reinterpret_cast<Entity*>(1));
		//present.hasSheep(0);
		//_tree.train(13, _tree.randomAction(), present, other, 0, 0, 0, REPRODUCE);
		//_tree.train(14, _tree.randomAction(), present, 0, other, 0, 0, REPRODUCE);
		//_tree.train(15, _tree.randomAction(), present, 0, 0, 0, other, REPRODUCE);

		//other.hasWolf(reinterpret_cast<Entity*>(1));
		//present.hasSheep(reinterpret_cast<Entity*>(1));
		//_tree.train(15, _tree.randomAction(), present, 0, other, 0, 0, REPRODUCE);
		//_tree.train(14, _tree.randomAction(), present, 0, 0, 0, other, REPRODUCE);
		//_tree.train(13, _tree.randomAction(), present, 0, 0, other, 0, REPRODUCE);

		//_tree.generateTree();
	}

	void Wolf::sendXp(float power) 
	{
		while (_actionStack.size()) {
			_tree.train(_actionStack.top(), power);
			_actionStack.pop();
		}
		_tree.generateTree();
	}

	void Wolf::sendXpNot(float power) {
		while (_actionStack.size()) {
			_tree.trainNot(_actionStack.top(), power);
			_actionStack.pop();
		}
	}

	Action Wolf::getNewAction() 
	{
		EntityAction act = computeAction();
		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (moy > 0) {
				_lastMoy = moy;
				if (_validScore(moy)) {
					sendXp(1.5f);
				} else {
					sendXp(1.f);
				}
			} else {
				sendXpNot(0.3f);
			}
			reInitPerf();
		}
		return _actionArray[act];
	}

	void Wolf::eat(Board& board) 
	{
		if (isAlive() && _getSquare(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			std::cout << "Wolf eat" << std::endl;
			Entity* sheep =_getSquare(_loc).getEntity(Square::SHEEP);
			if (sheep) {
				sheep->setFood(0);
			}
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive() && _rep_limit > REPRODUCE_COUNTER && hasWolfNext() && _popEntity(_loc) && _popEntity(_loc) && _popEntity(_loc)) {
			_numberRep += 2;
			_rep_limit = REPRODUCE_COUNTER / 2;
			std::cout << "Wolf reproduce" << std::endl;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().cb_onEntityReproduce(*this);
		}
		generateNewAction();
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
