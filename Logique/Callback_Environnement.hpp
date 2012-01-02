
#ifndef CALLBACK_ENVIRONNEMENT
#define CALLBACK_ENVIRONNEMENT

#include <boost/bind.hpp>

#include "Entity.hpp"
#include "Board.hpp"
#include "Singleton.hpp"

namespace Logique {

	class Callback_Environnement : public Singleton< Callback_Environnement > {
		SINGLETON_CLASS(Callback_Environnement);
	public:
		typedef boost::function< void (const Entity&) > EntityFunctor;
		typedef boost::function< void (const Board&) > BoardFunctor;
		typedef boost::function< void (float, float) > MoyFunctor;

		void setSpawnSheep(const EntityFunctor& onSpawnSheep);
		void setSpawnWolf(const EntityFunctor& onSpawnWolf);
		void setOnEntityMove(const EntityFunctor& onEntityMove);
		void setOnReproduce(const EntityFunctor& onEntityMove);
		void setOnEntityEat(const EntityFunctor& onEntityEat);
		void setOnEntityDead(const EntityFunctor& onEntityDead);
		void setOnBoardChange(const BoardFunctor& onBoardChange);
		void setSendMoy(const MoyFunctor& SendMoyCallback);

		void cb_sendMoy(const float& sheep, const float& wolf) const;
		void cb_onBoardChange(const Board& value) const;
		void cb_onEntityDeath(const Entity& value) const;
		void cb_onSheepSpawn(const Entity& value) const;
		void cb_onWolfSpawn(const Entity& value) const;
		void cb_onEntityMove(const Entity& value) const;
		void cb_onEntityReproduce(const Entity& value) const;
		void cb_onEntityEat(const Entity& value) const;

	private:
		Callback_Environnement();
		~Callback_Environnement();
		Callback_Environnement(const Callback_Environnement& value);

		//functor
		BoardFunctor _onBoardChange;
		EntityFunctor _onEntityDeath;
		EntityFunctor _onSheepSpawn;
		EntityFunctor _onWolfSpawn;
		EntityFunctor _onEntityEat;
		EntityFunctor _onEntityReproduce;
		EntityFunctor _onEntityMove;
		MoyFunctor _sendMoyCallback;
	};

}

#endif /* ! CALLBACK_ENVIRONNEMENT */