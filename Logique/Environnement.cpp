
#include <iostream>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Environnement.hpp"
#include "Logger.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"

namespace Logique {

	Environnement::Environnement()
		: _board(), _baseTime(), _entityList(), _listMtx(), _attriMtx(), _actionList(), _actionTmpStack()
		, _randomD(), _gen(_randomD), _distri(0, BOARD_SIZE - 1)
	{
		_entityNum[Square::SHEEP] = 0;
		_entityNum[Square::WOLF] = 0;
		_baseTime = boost::posix_time::milliseconds(500);
		addAction(createBoardPlay());
		addAction(createLog());
	}

	Environnement::~Environnement() {}

	void Environnement::preRun() {
		addSheep(20);
		addWolf(20);

		//Wolf::initExp();
		//Sheep::initExp();

		Logger log("Loup.csv");
		log.wipeFile();
		Logger logM("Mouton.csv");
		logM.wipeFile();

		insertActionStack();

		if (_actionList.empty()) {
			std::cout << "no action" << std::endl;
		}
	}

	void Environnement::run() {
		preRun();

		unsigned int tickwait;
		boost::posix_time::time_duration sleep_time;
		boost::posix_time::time_duration zero;
		ActionList::iterator executor;
		ActionList::iterator toDeleteEnd;
		ActionList::iterator end;
		while (!_actionList.empty()) {
			tickwait = _actionList.begin()->_tickBeforeAction;
			sleep_time =  _baseTime * tickwait;
			if (sleep_time != zero) {
				boost::this_thread::sleep(sleep_time);
			}

			executor = _actionList.begin();
			end = _actionList.end();

			//on execute toute les action qui se sont deroulé pendant l'attente
			while (executor != end && executor->_tickBeforeAction <= tickwait) {
				executor->_action();
				++executor;
			}

			toDeleteEnd = executor;

			//les action restante voient leurs temps avant execution diminuer
			while (executor != end) {
				executor->_tickBeforeAction -= tickwait;
				++executor;
			}

			_actionList.erase(_actionList.begin(), toDeleteEnd);

			//on insere les nouvelle actions
			insertActionStack();

			if (getSheepNum() <= 3) {
				addSheep(15);
			}
			if (getWolfNum() <= 3) {
				addWolf(15);
			}
		}

		std::cout << "Simulation end" << std::endl;
	}

	void Environnement::setBaseTime(const boost::posix_time::time_duration& time) 
	{
		_baseTime = time;
	}

	//spawn Entity function
	bool Environnement::addSheep(const Coord& loc) 
	{
		if (!_board(loc).hasSheep()) {
			//std::cout << "spawn sheep at " << loc << std::endl;
			boost::shared_ptr<Sheep> sheepPtr(new Sheep());
			sheepPtr->addFood(Logique::FOOD_START);
			sheepPtr->initActionArray(_board);

			sheepPtr->setGetNumberSpecies(boost::bind(&Environnement::getSheepNum, this));
			sheepPtr->setPopEntityFunctor(boost::bind(&Environnement::popSheepNear, this, _1));
			initEntity(sheepPtr, loc);
			Callback_Environnement::getInstance().cb_onSheepSpawn(*sheepPtr);
			Callback_Environnement::getInstance().cb_onBoardChange(_board);
			return true;
		}
		return false;
	}

	bool Environnement::addWolf(const Coord& loc) 
	{
		if (!_board(loc).hasWolf()) {
			//std::cout << "spawn wolf at " << loc << std::endl;
			boost::shared_ptr<Wolf> wolfPtr(new Wolf());
			wolfPtr->addFood(Logique::FOOD_START);
			wolfPtr->initActionArray(_board);

			wolfPtr->setGetNumberSpecies(boost::bind(&Environnement::getWolfNum, this));
			wolfPtr->setPopEntityFunctor(boost::bind(&Environnement::popWolfNear, this, _1));
			initEntity(wolfPtr, loc);
			Callback_Environnement::getInstance().cb_onWolfSpawn(*wolfPtr);
			Callback_Environnement::getInstance().cb_onBoardChange(_board);
			return true;
		}
		return false;
	}

	void Environnement::addSheep(unsigned int num) 
	{
		Coord loc;
		unsigned int i = 0;

		while (i < num) {
			unsigned int limit = 0;

			do {
				loc = Coord(_distri(_gen), _distri(_gen));
			} while (_board(loc).hasSheep() && limit < 10);

			if (limit < 10)
				addSheep(loc);
			++i;
		}
	}

	void Environnement::addWolf(unsigned int num) 
	{
		Coord loc;
		unsigned int i = 0;

		while (i < num) {
			unsigned int limit = 0;

			do {
				loc = Coord(_distri(_gen), _distri(_gen));
			} while (_board(loc).hasWolf() && limit < 10);

			if (limit < 10)
				addWolf(loc);
			++i;
		}
	}

	bool Environnement::popSheepNear(const Coord& loc) 
	{
		bool found = false;
		Coord locFound;

		for (int x = loc.x - 1; x <= loc.x + 1 && !found; ++x) {
			for (int y = loc.y - 1; y <= loc.y + 1 && !found; ++y) {
				found = !_board.get(x, y).hasSheep();
				if (found) {
					locFound = _board.getValidValue(x, y);
				}
			}
		}

		return found && addSheep(locFound);
	}

	bool Environnement::popWolfNear(const Coord& loc) 
	{
		bool found = false;
		Coord locFound;

		for (int x = loc.x - 1; x <= loc.x + 1 && !found; ++x) {
			for (int y = loc.x - 1; y <= loc.y + 1 && !found; ++y) {
				found = !_board.get(x, y).hasWolf();
				if (found) {
					locFound = _board.getValidValue(x, y);
				}
			}
		}

		return found && addWolf(locFound);
	}

	unsigned int Environnement::getSheepNum() const 
	{
		return _entityNum[Square::SHEEP];
	}

	unsigned int Environnement::getWolfNum() const 
	{
		return _entityNum[Square::WOLF];
	}

	bool Environnement::validPerf(const float& value, Square::EntityContain type)
	{
		float moy = 0;
		float size = 0;
		EntityPtrSet::const_iterator it = _entityList.begin();
		EntityPtrSet::const_iterator ite = _entityList.end();

		while (it != ite) {
			if (it->first->getType() == type) {
				moy += it->first->getLastMoy();
				++size;
			}
			++it;
		}

		if (size)
			moy /= size;

		return  value && (value >= moy);
	}

	const Environnement::EntityPtrSet& Environnement::getEntityList() const 
	{
		return _entityList;
	}

	void Environnement::lock() 
	{
		_attriMtx.lock();
	}

	void Environnement::unlock() 
	{
		_attriMtx.unlock();
	}

	void Environnement::addAction(const Action& value) 
	{
		_listMtx.lock(); // ##list lock
		_actionTmpStack.push(value);
		_listMtx.unlock(); // ## list unlock
	}

	void Environnement::unsafeInsertAction(const Action& value) 
	{
		ActionList::iterator it = _actionList.begin();
		ActionList::iterator ite = _actionList.end();

		while (it != ite && it->_tickBeforeAction < value._tickBeforeAction) {
			++it;
		}
		_actionList.insert(it, value);
	}

	void Environnement::insertActionStack() 
	{
		_listMtx.lock(); // ##list lock
		while (_actionTmpStack.size()) {
			unsafeInsertAction(_actionTmpStack.top());
			_actionTmpStack.pop();
		}
		_listMtx.unlock(); // ## list unlock
	}

	Action Environnement::createBoardPlay() 
	{
		Action act;

		act._action = boost::bind(&Environnement::boardPlay, this);
		act._tickBeforeAction = 1;
		return act;
	}

	Action Environnement::createLog()
	{
		Action act;

		act._action = boost::bind(&Environnement::doLog, this);
		act._tickBeforeAction = 2 * 30;
		return act;
	}

	void Environnement::doLog()
	{
		float totalSheep = 0;
		float totalWolf = 0;

		EntityPtrSet::const_iterator it = _entityList.begin();
		EntityPtrSet::const_iterator ite = _entityList.end();

		while (it != ite)
		{
			if (it->first->getType() == Square::SHEEP)
				totalSheep += it->first->getLastMoy();
			else
				totalWolf += it->first->getLastMoy();

			++it;
		}
		totalSheep /= static_cast<float>(_entityNum[Square::SHEEP]);
		totalWolf /= static_cast<float>(_entityNum[Square::WOLF]);

		Logger sheepLog("Mouton.csv");
		sheepLog.dump(totalSheep);
		Logger wolfLog("Loup.csv");
		wolfLog.dump(totalWolf);

		Callback_Environnement::getInstance().cb_sendMoy(totalSheep, totalWolf);
	}

	void Environnement::boardPlay() {
		Coord grassSpawn;
		unsigned int odour_higher = 0;

		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				if (_board[x][y].odour() > odour_higher && !_board[x][y].hasGrass()) {
					odour_higher = _board[x][y].odour();
					grassSpawn.x = x;
					grassSpawn.y = y;
				}
				
				_board[x][y].decreaseOdour(1);
			}
		}

		if (!odour_higher) {
			grassSpawn = Coord(_distri(_gen), _distri(_gen));
		} 
		/*else {
			_board.dump();
		}*/

		//std::cout << "try spawn grass on " << grassSpawn << std::endl;
		if (!_board(grassSpawn).hasGrass()) {
			//std::cout << "spawn grass on " << grassSpawn << std::endl;
			_board.lock();
			_board(grassSpawn).hasGrass(true);
			int value = _board(grassSpawn).getInt();
			//std::cout << "case value " << value << std::endl;
			_board.unlock();
			Callback_Environnement::getInstance().cb_onBoardChange(_board);
		}

		addAction(createBoardPlay());
	}

	void Environnement::initEntity(boost::shared_ptr<Entity> value, const Coord& loc) {
		_board.lock();
		_board(loc).hasEntity(value->getType(), value.get());
		_board.unlock();
		_attriMtx.lock();
		_entityList[value.get()] = value;
		_entityNum[value->getType()]++;
		_attriMtx.unlock();
		value->setLocation(loc);
		value->setValidScore(boost::bind(&Environnement::validPerf, this, _1, value->getType()));
		value->setAddAction(boost::bind(&Environnement::addAction, this, _1));
		value->setOnDeath(boost::bind(&Environnement::onEntityDeath, this, _1));
		value->setGetSquare(boost::bind(static_cast<board_func>(&Board::get), &_board, _1));
		value->reInitPerf();
		addAction(value->createFoodAction());
		addAction(value->getNewAction());
	}

	void Environnement::spawnSheep() {
		Coord loc;
		unsigned int limit = 0;

		do {
			loc = Coord(_distri(_gen), _distri(_gen));
		} while (_board(loc).hasSheep() && limit < 10);

		if (limit < 10)
			addSheep(loc);
	}

	void Environnement::onEntityDeath(Entity& value) {
		Callback_Environnement::getInstance().cb_onEntityDeath(value);

		_board.lock();
		popOdour(value.getLocation());
		_board(value.getLocation()).hasEntity(value.getType(), 0);
		_board.unlock();
		
		_attriMtx.lock();
		_entityNum[value.getType()]--;		
		_attriMtx.unlock();

		Callback_Environnement::getInstance().cb_onBoardChange(_board);
		value.cleanVtable();
		EntityPtrSet::iterator it = _entityList.find(&value);
		if (it != _entityList.end()) {
			_attriMtx.lock();
			_entityList.erase(it);
			_attriMtx.unlock();
		}
	}

	void Environnement::popOdour(const Coord& loc, unsigned int power) {
		for (unsigned int size = power; size > 0; size--) {
			int x_start = loc.x - size + 1;
			int y_start;
			int x_end = loc.x + size;
			int y_end = loc.y + size;

			while (x_start < x_end) {
				y_start = loc.y - size + 1;

				while (y_start < y_end) {
					addOdour(x_start, y_start, 1);
					y_start++;
				}

				x_start++;
			}
		}
	}

	void Environnement::addOdour(int x, int y, unsigned int value) {
		_board.get(x, y).addOdour(value);
	}


}
