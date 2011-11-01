#include "NetworkEnvironnement.hpp"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <stdint.h>
#include <sstream>

namespace Wrapper
{
	NetworkEnvironnement::NetworkEnvironnement(Networking::Server * server) : env_(), server_(server)
	{
		server_->setSynchronize(boost::bind(&NetworkEnvironnement::synchronize, this, _1));
		env_.setSpawnSheep(boost::bind(&NetworkEnvironnement::onSpawnSheep, this, _1));
		env_.setSpawnWolf(boost::bind(&NetworkEnvironnement::onSpawnWolf, this, _1));
		env_.setOnEntityMove(boost::bind(&NetworkEnvironnement::onEntityMove, this, _1));
		env_.setOnReproduce(boost::bind(&NetworkEnvironnement::onReproduce, this, _1));
		env_.setOnEntityEat(boost::bind(&NetworkEnvironnement::onEntityEat, this, _1));
		env_.setOnEntityDead(boost::bind(&NetworkEnvironnement::onEntityDead, this, _1));
		env_.setOnBoardChange(boost::bind(&NetworkEnvironnement::onBoardChange, this, _1));
		boost::thread env_thr(boost::bind(&Logique::Environnement::run, &env_));
	}

	NetworkEnvironnement::~NetworkEnvironnement(void)
	{
	}

	void NetworkEnvironnement::synchronize(Networking::Server::socket_ptr & socket)
	{
		std::clog << "[Log] synchronize... entity..." << std::endl;
		env_.lock();
		Logique::Environnement::EntityPtrSet::const_iterator it = env_.getEntityList().begin();
		Logique::Environnement::EntityPtrSet::const_iterator ite = env_.getEntityList().end();
		for (; it != ite; ++it)
		{
			Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

			std::stringstream sstream;
			if (it->first->getType() == Logique::Square::SHEEP)
				sstream << SPAWN << ";s;" << (uint32_t)(it->first) << ";" << it->first->getLocation().x << ";" << it->first->getLocation().y;
			else
				sstream << SPAWN << ";w;" << (uint32_t)(it->first) << ";" << it->first->getLocation().x << ";" << it->first->getLocation().y;
			package->init(sstream.str());
			if (!server_->send_message(socket, package)) break;
		}
		env_.unlock();
		std::clog << "[Log] synchronize finished..." << std::endl;
	}

	void NetworkEnvironnement::onSpawnSheep(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::stringstream sstream;
		sstream << SPAWN << ";s;" << (uint32_t)&entity << ";" << entity.getLocation().x << ";" << entity.getLocation().y;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}

	void NetworkEnvironnement::onSpawnWolf(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::stringstream sstream;
		sstream << SPAWN << ";w;" << (uint32_t)&entity << ";" << entity.getLocation().x << ";" << entity.getLocation().y;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}

	void NetworkEnvironnement::onEntityMove(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::stringstream sstream;
		sstream << MOVE << ";" << (uint32_t)&entity << ";" << entity.getLastAction() << ";" << entity.getLocation().x << ";" << entity.getLocation().y;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}

	void NetworkEnvironnement::onEntityEat(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::stringstream sstream;
		sstream << EAT << ";" << (uint32_t)&entity;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}

	void NetworkEnvironnement::onEntityDead(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::stringstream sstream;
		sstream << DIE << ";" <<  (uint32_t)&entity;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}

	void NetworkEnvironnement::onReproduce(Logique::Entity const & entity)
	{
		Networking::Server::Package_ptr package = Networking::Server::Package_ptr(new Networking::Package());

		std::cout << "[SERVER] REPRODUCE" << std::endl;
		std::stringstream sstream;
		sstream << CLONE << ";" <<  (uint32_t)&entity;
		package->init(sstream.str());
		server_->add_sending_package(package);
	}
	
	void NetworkEnvironnement::onBoardChange(Logique::Board const & board)
	{
		const_cast<Logique::Board&>(board).lock();
		Networking::Server::Package_queue packages;

		Networking::Package * begin = new Networking::Package();
		std::stringstream sstreamb;
		sstreamb << BOARD_BEG << ";" << board.size();
		begin->init(sstreamb.str());
		packages.push_back(Networking::Server::Package_ptr(begin));
		for (uint32_t x = 0; x < board.size(); x++)
		{
			for (uint32_t y = 0; y < board.size(); y++)
			{
				std::stringstream sstreamp;
				Networking::Package * pack = new Networking::Package();
				sstreamp << BOARD << ";" << x << ";" << y << ";" << (int)board.get(Coord(x, y)).hasGrass() << ";" << board.get(Coord(x, y)).odour();
				pack->init(sstreamp.str());
				packages.push_back(Networking::Server::Package_ptr(pack));
			}
		}
		Networking::Package * end = new Networking::Package();
		std::stringstream sstreame;
		sstreame << BOARD_END;;
		end->init(sstreame.str());
		packages.push_back(Networking::Server::Package_ptr(end));
		server_->add_sending_packages(packages);
		const_cast<Logique::Board&>(board).unlock();
	}
}
