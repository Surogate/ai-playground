
#include "Wolf.hpp"
#include "Board.hpp"
#include "Callback_Environnement.hpp"
#include "Logger.hpp"

namespace Logique {

	DecisionTree Wolf::_tree;

	Wolf::Wolf() 
		: Entity(Square::WOLF)
	{}

	Wolf::~Wolf() {
	}

	void Wolf::initActionArray(Board& board) {
		Entity::initActionArray(board);
		_actionArray[EAT] = Action(EAT_TIME, boost::bind(&Entity::eat, shared_from_this(), boost::ref(board)));
		_actionArray[REPRODUCE] = Action(REPRODUCE_TIME, boost::bind(&Entity::reproduce, shared_from_this(), boost::ref(board)));
	}



	Entity::EntityAction Wolf::computeAction() {
		EntityAction act = _tree.computeAction(_foodCount, _getSquare(_loc), getSquareLess(_loc, Coord::DOWN), getSquareLess(_loc, Coord::RIGHT), getSquareSup(_loc, Coord::DOWN), getSquareSup(_loc, Coord::RIGHT));
		_actionStack.push(ActionStore(_foodCount, _getSquare(_loc),  getSquareLess(_loc, Coord::DOWN), getSquareLess(_loc, Coord::RIGHT), getSquareSup(_loc, Coord::DOWN), getSquareSup(_loc, Coord::RIGHT), act, _lastAction));
		_actual++;
		return act;
	}

	void Wolf::initExp() {
		Square present;
		present.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		Square other;
		other.hasGrass(true);

		_tree.train(0, present, other, other, other, other, EAT);
		other.hasGrass(false);
		_tree.train(1, present, other, other, other, other, EAT);
		other.hasSheep(0);
		_tree.train(2, present, other, other, other, other, EAT);
		other.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		_tree.train(3, present, 0, other, other, other, EAT);
		other.addOdour(1);
		_tree.train(4, present, other, 0, other, other, EAT);
		other.addOdour(1);
		_tree.train(5, present, other, other, other, 0, EAT);
		other.addOdour(1);
		_tree.train(6, present, other, other, 0, other, EAT);
		other.addOdour(1);
		other.hasGrass(true);
		_tree.train(7, present, 0, other, other, 0, EAT);
		other.addOdour(1);
		_tree.train(8, present, other, other, other, other, EAT);
		other.addOdour(10);
		_tree.train(9, present, other, 0, 0, other, EAT);

		other.hasWolf(reinterpret_cast<Entity*>(1));
		present.hasSheep(0);
		_tree.train(13, present, other, 0, 0, 0, REPRODUCE);
		_tree.train(14, present, 0, other, 0, 0, REPRODUCE);
		_tree.train(15, present, 0, 0, 0, other, REPRODUCE);

		other.hasWolf(reinterpret_cast<Entity*>(1));
		present.hasSheep(reinterpret_cast<Entity*>(1));
		_tree.train(15, present, 0, other, 0, 0, REPRODUCE);
		_tree.train(14, present, 0, 0, 0, other, REPRODUCE);
		_tree.train(13, present, 0, 0, other, 0, REPRODUCE);

		_tree.generateTree();
	}


	void Wolf::sendXp() {
		while (_actionStack.size()) {
					ActionStore& top = _actionStack.top();
					_tree.train(top.foodcount, top.present, top.up, top.left, top.down, top.right, top.result);
					_actionStack.pop();
		}
		
		_tree.generateTree();
	}

	Action Wolf::getNewAction() {
		EntityAction act = computeAction();
		if (_actual && _actual >= _numberTot) {
			float moy = computeMoy();

			if (_validScore(moy)) {
				sendXp();
				Logger log("Loup.csv");
				std::cout << "#Wolf action commited - old perf " << _tree.getMoy() << std::endl;
				std::cout << "#Wolf new perf " << moy << std::endl;
				std::cout << "#Wolf experience size " << _tree.getSize() << std::endl;
				_lastMoy = moy;
				_tree.sendMoy(moy);
				log.dump(_tree.getMoy());
			}
			reInitPerf();
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
				sheep->setFood(0);
			}
			addFood(FOOD_GAIN);
			Callback_Environnement::getInstance().cb_onEntityEat(*this);
		}
		generateNewAction();
	}

	void Wolf::reproduce(Board& board) {
		if (isAlive() && _foodCount > _rep_limit && hasWolfNext() && _popEntity(_loc)) {
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
