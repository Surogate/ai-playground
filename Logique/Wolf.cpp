
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
		int up = getIntFromLess(_loc, Coord::DOWN);
		int left = getIntFromLess(_loc, Coord::RIGHT);
		int down = getIntFromSup(_loc, Coord::DOWN);
		int right = getIntFromSup(_loc, Coord::RIGHT);
		EntityAction act = _tree.computeAction(_foodCount, _getSquare(_loc), up, left, down, right); 
		_actionStack.push(ActionStore(_foodCount, _getSquare(_loc), up, left, down, right, act));
		_actual++;
		return act;
	}

	void Wolf::initExp() {
		Square present;
		present.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		Square other;
		other.hasGrass(true);

		_tree.addAction(0, present, other, other, other, other, EAT);
		other.hasGrass(false);
		_tree.addAction(1, present, other, other, other, other, EAT);
		other.hasSheep(0);
		_tree.addAction(2, present, other, other, other, other, EAT);
		other.hasSheep(reinterpret_cast<Logique::Entity*>(1));
		_tree.addAction(3, present, 0, other, other, other, EAT);
		other.addOdour(1);
		_tree.addAction(4, present, other, 0, other, other, EAT);
		other.addOdour(1);
		_tree.addAction(5, present, other, other, other, 0, EAT);
		other.addOdour(1);
		_tree.addAction(6, present, other, other, 0, other, EAT);
		other.addOdour(1);
		other.hasGrass(true);
		_tree.addAction(7, present, 0, other, other, 0, EAT);
		other.addOdour(1);
		_tree.addAction(8, present, other, other, other, other, EAT);
		other.addOdour(10);
		_tree.addAction(9, present, other, 0, 0, other, EAT);

		other.hasWolf(reinterpret_cast<Entity*>(1));
		present.hasSheep(0);
		_tree.addAction(13, present, other, 0, 0, 0, REPRODUCE);
		_tree.addAction(14, present, 0, other, 0, 0, REPRODUCE);
		_tree.addAction(15, present, 0, 0, 0, other, REPRODUCE);

		other.hasWolf(reinterpret_cast<Entity*>(1));
		present.hasSheep(reinterpret_cast<Entity*>(1));
		_tree.addAction(15, present, 0, other, 0, 0, REPRODUCE);
		_tree.addAction(14, present, 0, 0, 0, other, REPRODUCE);
		_tree.addAction(13, present, 0, 0, other, 0, REPRODUCE);

		_tree.generateTree();
	}


	void Wolf::sendXp() {
		while (_actionStack.size()) {
					ActionStore& top = _actionStack.top();
					_tree.addAction(top.foodcount, top.present, top.up, top.left, top.down, top.right, top.result);
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
				Logger log("Loup.log");
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
