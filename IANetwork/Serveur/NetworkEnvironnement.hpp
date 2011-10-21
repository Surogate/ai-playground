#ifndef _NETWORK_ENVIRONNEMENT_HPP_
#define _NETWORK_ENVIRONNEMENT_HPP_

#include "Server.hpp"
#include "Environnement.hpp"

namespace Wrapper
{
	class NetworkEnvironnement
	{
	public:
		enum COMMAND {
			SPAWN,
			MOVE,
			EAT,
			DIE,
			STARVING,
			CLONE,
			BOARD_BEG,
			BOARD,
			BOARD_END
		};

		NetworkEnvironnement(Networking::Server * server);
		~NetworkEnvironnement(void);

	private:
		Logique::Environnement	env_;
		Networking::Server *	server_;

		void synchronize(Networking::Server::socket_ptr &);
		void onSpawnSheep(Logique::Entity const &);
		void onSpawnWolf(Logique::Entity const &);
		void onEntityMove(Logique::Entity const &);
		void onEntityEat(Logique::Entity const &);
		void onEntityDead(Logique::Entity const &);
		void onReproduce(Logique::Entity const &);
		void onBoardChange(Logique::Board const &);
	};
}

#endif