#include "NetworkEnvironnement.hpp"

namespace Wrapper
{
	NetworkEnvironnement::NetworkEnvironnement(Networking::Server * server) : env_(), server_(server)
	{
		env_.setSpawnSheep(boost::bind(&NetworkEnvironnement::onSpawnSheep, this, _1));
		env_.setSpawnWolf(boost::bind(&NetworkEnvironnement::onSpawnWolf, this, _1));
		env_.setOnEntityMove(boost::bind(&NetworkEnvironnement::onEntityMove, this, _1));
		env_.setOnReproduce(boost::bind(&NetworkEnvironnement::onReproduce, this, _1));
		env_.setOnEntityEat(boost::bind(&NetworkEnvironnement::onEntityEat, this, _1));
		env_.setOnEntityDead(boost::bind(&NetworkEnvironnement::onEntityDead, this, _1));
		env_.setOnBoardChange(boost::bind(&NetworkEnvironnement::onBoardChange, this, _1));
	}

	NetworkEnvironnement::~NetworkEnvironnement(void)
	{
	}

	void NetworkEnvironnement::onSpawnSheep(Logique::Entity const & entity)
	{
	}

	void NetworkEnvironnement::onSpawnWolf(Logique::Entity const & entity)
	{
	}

	void NetworkEnvironnement::onEntityMove(Logique::Entity const & entity)
	{
	}

	void NetworkEnvironnement::onEntityEat(Logique::Entity const & entity)
	{
	}

	void NetworkEnvironnement::onEntityDead(Logique::Entity const & entity)
	{
	}

	void NetworkEnvironnement::onReproduce(Logique::Entity const & entity)
	{
	}
	
	void NetworkEnvironnement::onBoardChange(Logique::Board const & board)
	{

	}
}
