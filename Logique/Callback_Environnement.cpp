
#include "Callback_Environnement.hpp"

namespace Logique {

	Callback_Environnement::Callback_Environnement() 
		: _onBoardChange(), _onEntityDeath(), _onSheepSpawn(), _onWolfSpawn()
		, _onEntityEat(), _onEntityReproduce(), _onEntityMove()
	{}

	Callback_Environnement::~Callback_Environnement() {}

	Callback_Environnement::Callback_Environnement(const Callback_Environnement& orig) {}

	void Callback_Environnement::setSpawnSheep(const EntityFunctor& onSpawnSheep) {
		_onSheepSpawn = onSpawnSheep;
	}

	void Callback_Environnement::setSpawnWolf(const EntityFunctor& onSpawnWolf) {
		_onWolfSpawn = onSpawnWolf;
	}
	
	void Callback_Environnement::setOnEntityMove(const EntityFunctor& onEntityMove) {
		_onEntityMove = onEntityMove;
	}

	void Callback_Environnement::setOnReproduce(const EntityFunctor& onEntityRep) {
		_onEntityReproduce = onEntityRep;
	}

	void Callback_Environnement::setOnEntityEat(const EntityFunctor& onEntityEat) {
		_onEntityEat = onEntityEat;
	}

	void Callback_Environnement::setOnEntityDead(const EntityFunctor& onEntityDead) {
		_onEntityDeath = onEntityDead;
	}

	void Callback_Environnement::setOnBoardChange(const BoardFunctor& onBoardChange) {
		_onBoardChange = onBoardChange;
	}

	void Callback_Environnement::cb_onBoardChange(const Board& value) const { 
		if (_onBoardChange) _onBoardChange(value); 
	}
	
	void Callback_Environnement::cb_onEntityDeath(const Entity& value) const {
		if (_onEntityDeath) _onEntityDeath(value); 
	}

	void Callback_Environnement::cb_onSheepSpawn(const Entity& value) const {
		if (_onSheepSpawn) _onSheepSpawn(value);
	}

	void Callback_Environnement::cb_onWolfSpawn(const Entity& value) const {
		if (_onWolfSpawn) _onWolfSpawn(value);
	}

	void Callback_Environnement::cb_onEntityMove(const Entity& value) const {
		if (_onEntityMove) _onEntityMove(value);
	}

	void Callback_Environnement::cb_onEntityReproduce(const Entity& value) const {
		if (_onEntityReproduce) _onEntityReproduce(value);
	}

	void Callback_Environnement::cb_onEntityEat(const Entity& value) const {
		if (_onEntityEat) _onEntityEat(value);
	}
}

