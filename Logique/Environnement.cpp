
#include <iostream>

#include <boost/foreach.hpp>

#include "Environnement.hpp"
#include "Logger.hpp"
#include "Sheep.hpp"
#include "Wolf.hpp"

namespace Logique {

	Environnement::Environnement()
		: _board(), _baseTime(), _entityList(), _listMtx(), _attriMtx(), _actionList(), _actionTmpStack()
		, _randomD(), _gen(_randomD), _distri(0, 15)
	{
		_entityNum[Square::SHEEP] = 0;
		_entityNum[Square::WOLF] = 0;
		_baseTime = boost::chrono::milliseconds(1000);
		addAction(createBoardPlay());
	}

	Environnement::~Environnement() {}

	void Environnement::preRun() {
		addSheep(20);
		addWolf(10);

		insertActionStack();

		if (_actionList.empty()) {
			std::cout << "no action" << std::endl;
		}
	}

	void Environnement::run() {
		preRun();

		boost::chrono::system_clock::time_point start;
		boost::chrono::duration<double> total_time;

		ActionList::iterator executor;
		ActionList::iterator toDeleteEnd;
		ActionList::iterator end;

		std::cout << "simulation start" << std::endl;
		while (!_actionList.empty()) {
			start = boost::chrono::system_clock::now();

			unsigned int tick_passed = std::floor(total_time.count() / _baseTime.count());

			if (tick_passed > 0) {
				total_time -= (tick_passed * _baseTime);

				executor = _actionList.begin();
				end = _actionList.end();

				while (executor != end && executor->increment(tick_passed)) {
					++executor;
				}

				toDeleteEnd = executor;
				executor++;

				while (executor != end)
				{
					executor->increment(tick_passed);
					executor++;
				}

				if (toDeleteEnd != _actionList.begin())
					_actionList.erase(_actionList.begin(), toDeleteEnd);

			}

			if (getSheepNum() <= 3) {
				addSheep(20);
			}

			if (getWolfNum() <= 3) {
				addWolf(10);
			}

			insertActionStack();

			if (total_time < _baseTime) 
				boost::this_thread::sleep(boost::posix_time::seconds(_baseTime.count()));

			total_time += boost::chrono::system_clock::now() - start;
		}
		std::cout << "Simulation end" << std::endl;
	}

	void Environnement::setBaseTime(const boost::chrono::duration<double>& time) 
	{
		_baseTime = time;
	}

	//spawn Entity function
	bool Environnement::addSheep(const Coord& loc) 
	{
		if (!_board(loc).hasSheep()) {
			boost::shared_ptr<Sheep> sheepPtr(_sheepPool.construct(), boost::bind(&Environnement::destroySheep, this, _1));
			sheepPtr->addFood(Logique::FOOD_START);
			sheepPtr->initActionArray(_board);

			sheepPtr->setGetNumberSpecies(boost::bind(&Environnement::getSheepNum, this));
			sheepPtr->setPopEntityFunctor(boost::bind(&Environnement::popSheepNear, this, _1));
			initEntity(sheepPtr, loc);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_SPAWN, *sheepPtr.get(), Square::SHEEP, loc);
			sheepPtr->getNewAction(0);
			return true;
		}
		return false;
	}

	bool Environnement::addWolf(const Coord& loc) 
	{
		if (!_board(loc).hasWolf()) {
			boost::shared_ptr<Wolf> wolfPtr(_wolfPool.construct(), boost::bind(&Environnement::destroyWolf, this, _1));
			wolfPtr->addFood(Logique::FOOD_START);
			wolfPtr->initActionArray(_board);

			wolfPtr->setGetNumberSpecies(boost::bind(&Environnement::getWolfNum, this));
			wolfPtr->setPopEntityFunctor(boost::bind(&Environnement::popWolfNear, this, _1));
			initEntity(wolfPtr, loc);
			Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_SPAWN, *wolfPtr.get(), Square::WOLF, loc);
			wolfPtr->getNewAction(0);
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

	unsigned int Environnement::getEntityNum(const Square::EntityContain& value) const
	{
		return _entityNum[value];
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

		while (it != ite && it->tickBefore() < value.tickBefore()) {
			++it;
		}
		if (it != ite) {
			_actionList.insert(it, value);
		} else {
			_actionList.push_back(value);
		}
	}

	void Environnement::debugActionList() {
		ActionList::iterator it = _actionList.begin();
		ActionList::iterator ite = _actionList.end();

		std::cout << "## size:" << _actionList.size() << std::endl;
		while (it != ite) {
			std::cout << "tick start:" << it->_tickStart << " tickEnd:" << it->_tickBeforeAction << " diff:" << it->tickBefore() << std::endl;
			++it;
		}
		std::cout << "##" << std::endl;
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

//		Callback_Environnement::getInstance().cb_sendMoy(totalSheep, totalWolf);
	}

	void Environnement::boardPlay() {
		for (unsigned int x = 0; x < BOARD_SIZE; ++x) {
			for (unsigned int y = 0; y < BOARD_SIZE; ++y) {
				Square& val = _board[x][y];
				if (!val.hasEntity() && val.increaseGrass())
					Callback_Environnement::getInstance().addAction(Environnement_Event::GRASS_UP, Coord(x, y));
				if (val.hasEntity() && val.decreaseGrass())
					Callback_Environnement::getInstance().addAction(Environnement_Event::GRASS_DOWN, Coord(x, y));
				val.decreaseOdour(1);
			}
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
		Callback_Environnement::getInstance().addAction(Environnement_Event::ENTITY_DEATH, value, value.getType(), value.getLocation());

		_board.lock();
		popOdour(value.getLocation());
		_board(value.getLocation()).hasEntity(value.getType(), 0);
		_board.unlock();
		
		_attriMtx.lock();
		_entityNum[value.getType()]--;		
		_attriMtx.unlock();

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

	void Environnement::destroySheep(Sheep* value) {
		_sheepPool.destroy(value);
	}

	void Environnement::destroyWolf(Wolf* value) {
		_wolfPool.destroy(value);
	}
}
