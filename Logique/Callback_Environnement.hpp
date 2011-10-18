
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

		Callback_Environnement() : onBoardChange_(), onEntityDeath_() {}

		void setSpawnSheep(const EntityFunctor& onSpawnSheep) {
		}

		void setSpawnWolf(const EntityFunctor& onSpawnWolf) {
		}

		void setOnEntityMove(const EntityFunctor& onEntityMove) {
		}

		void setOnReproduce(const EntityFunctor& onEntityMove) {
		}

		void setOnEntityEat(const EntityFunctor& onEntityEat) {
		}

		void setOnEntityDead(const EntityFunctor& onEntityDead) {
			onEntityDeath_ = onEntityDead;
		}

		void setOnBoardChange(const BoardFunctor& onBoardChange) {
			onBoardChange_ = onBoardChange;
		}

	protected:
		//functor
		BoardFunctor onBoardChange_;
		EntityFunctor onEntityDeath_;

		inline void cb_onBoardChange(const Board& value) const { if (onBoardChange_) onBoardChange_(value); }
		inline void cb_onEntityDeath(const Entity& value) const { if (onEntityDeath_) onEntityDeath_(value); }
	};

}

#endif /* ! CALLBACK_ENVIRONNEMENT */