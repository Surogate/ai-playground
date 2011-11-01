
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Wolf::_tree;

	Wolf::Wolf() 
		: Entity(Square::WOLF), _numberEat(0)
	{}

	void Wolf::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, boost::bind(&Entity::eat, shared_from_this(), boost::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, boost::bind(&Entity::reproduce, shared_from_this(), boost::ref(board)));
	}

	Entity::EntityAction Wolf::computeAction() {
		int present = _getSquare(_loc).getInt();
		int up = getIntFromLess(_loc, Coord::DOWN);
		int left = getIntFromLess(_loc, Coord::RIGHT);
		int down = getIntFromSup(_loc, Coord::DOWN);
		int right = getIntFromSup(_loc, Coord::RIGHT);
		EntityAction act = _tree.computeAction(present, up, left, down, right); 
		_actionStack.push(ActionStore(present, up, left, down, right, act));
		
		return act;
	}

	Action Wolf::getNewAction() {
		EntityAction act = computeAction();
		float moy = computeMoy();
		if (_actual && _actual >= _numberTot && computeMoy() >= _tree.getMoy()) {
			while (_actionStack.size()) {
				ActionStore& top = _actionStack.top();
				_tree.addAction(top.present, top.up, top.left, top.down, top.right, top.result);
				_actionStack.pop();
			}
			reInitPerf();
			Logger log("Loup.log");
			log.dump(moy);
			std::cout << "action commited - old perf " << _tree.getMoy() << std::endl;
			std::cout << "new perf " << moy << std::endl;
			std::cout << "experience size " << _tree.getSize() << std::endl;
			_tree.sendMoy(moy);
		}
		return _actionArray[act];
	}

	void Wolf::eat(Board& board) {
		if (isAlive() && _getSquare(_loc).hasSheep()) {
			_lastAction = EAT;
			_numberEat++;
			std::cout << "Wolf eat" << std::endl;
			Entity* sheep =_getSquare(_loc).getEntity(Square::SHEEP);
			if (sheep) {
				sheep->decreaseFood(Entity::FOOD_MAX);
			}
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive() && hasWolfNext() && _popEntity(_loc)) {
			_numberRep++;
			std::cout << "Wolf reproduce" << std::endl;
			_lastAction = REPRODUCE;
			Callback_Environnement::getInstance().cb_onEntityReproduce(*this);
		}
		generateNewAction();
	}

	bool Wolf::hasWolfNext() {
		int up = getIntFromLess(_loc, Coord::DOWN);
		int left = getIntFromLess(_loc, Coord::RIGHT);
		int down = getIntFromSup(_loc, Coord::DOWN);
		int right = getIntFromSup(_loc, Coord::RIGHT);
		return (up & Square::WOLF_MASK) 
			|| (left & Square::WOLF_MASK)
			|| (down & Square::WOLF_MASK)
			|| (right & Square::WOLF_MASK);
	}
}
