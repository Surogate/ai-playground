
#ifndef CALLBACK_ENVIRONNEMENT
#define CALLBACK_ENVIRONNEMENT

#include <boost/bind.hpp>

#include "Entity.hpp"
#include "Board.hpp"

namespace Logique {

	class Callback_Environnement {
	public:
		typedef boost::function< void (const Entity&) > EntityFunctor;
		typedef boost::function< void (const Board&) > BoardFunctor;

		Callback_Environnement();

		virtual ~Callback_Environnement();

		void setSpawnSheep(const EntityFunctor& onSpawnSheep);

		void setSpawnWolf(const EntityFunctor& onSpawnWolf);

		void setOnEntityMove(const EntityFunctor& onEntityMove);

		void setOnReproduce(const EntityFunctor& onEntityMove);

		void setOnEntityEat(const EntityFunctor& onEntityEat);

		void setOnEntityDead(const EntityFunctor& onEntityDead);

		void setOnBoardChange(const BoardFunctor& onBoardChange);

	protected:
		//functor
		BoardFunctor _onBoardChange;
		EntityFunctor _onEntityDeath;
		EntityFunctor _onSheepSpawn;
		EntityFunctor _onWolfSpawn;
		EntityFunctor _onEntityEat;
		EntityFunctor _onEntityReproduce;
		EntityFunctor _onEntityMove;

		void cb_onBoardChange(const Board& value) const;
		void cb_onEntityDeath(const Entity& value) const;
		void cb_onSheepSpawn(const Entity& value) const;
		void cb_onWolfSpawn(const Entity& value) const;
		void cb_onEntityMove(const Entity& value) const;
		void cb_onEntityReproduce(const Entity& value) const;
		void cb_onEntityEat(const Entity& value) const;
	};

}

#endif /* ! CALLBACK_ENVIRONNEMENT */