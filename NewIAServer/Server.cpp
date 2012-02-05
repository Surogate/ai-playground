/* 
 * File:   Server.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 4:54 PM
 */

#include "Server.hpp"
#include "Callback_Environnement.hpp"
#include <boost/chrono.hpp>
#include <iostream>

Server::Server() :
	socket_(16000),
	socket2_(16500),
	server_(new Poco::Net::TCPServerConnectionFactoryImpl<Connection>(),
	socket_),
	server2_(new Poco::Net::TCPServerConnectionFactoryImpl<Connection>(),
	socket2_),
	connections_(),
	connections_info_(),
	environnement_(),
	connections_mut_(),
	connections_info_mut_() {
		forge_[Logique::Environnement_Event::ENTITY_SPAWN] = boost::bind(&Server::spawnForge, this, _1, _2);
		forge_[Logique::Environnement_Event::ENTITY_MOVE] = boost::bind(&Server::moveForge, this, _1, _2);
		forge_[Logique::Environnement_Event::ENTITY_EAT] = boost::bind(&Server::eatForge, this, _1, _2);
		forge_[Logique::Environnement_Event::ENTITY_DEATH] = boost::bind(&Server::dieForge, this, _1, _2);
		forge_[Logique::Environnement_Event::ENTITY_REPRODUCE] = boost::bind(&Server::reproduceForge, this, _1, _2);
		forge_[Logique::Environnement_Event::GRASS_UP] = boost::bind(&Server::grassUpForge, this, _1, _2);
		forge_[Logique::Environnement_Event::GRASS_DOWN] = boost::bind(&Server::grassDownForge, this, _1, _2);
}

Server::~Server() {
}


void Server::Start()
{
    std::cout << "server start" << std::endl;
    server_.start();
	server2_.start();
	boost::thread thr(boost::bind(&Server::run, this));
    environnement_.run();
}

void Server::AddConnection(Connection* connection)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    connections_[reinterpret_cast<ptrdiff_t>(connection)] = connection; 
}

void Server::AddConnectionInfo(ConnectionInfo* connectionInfo)
{
	boost::lock_guard<boost::mutex> lock(connections_info_mut_);
    connections_info_[reinterpret_cast<ptrdiff_t>(connectionInfo)] = connectionInfo; 
}

void Server::RemoveConnection(Connection* connection)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    if (connections_.find(reinterpret_cast<ptrdiff_t>(connection)) != connections_.end())
        connections_.erase(reinterpret_cast<ptrdiff_t>(connection));
}

void Server::RemoveConnectionInfo(ConnectionInfo* connectionInfo)
{
	boost::lock_guard<boost::mutex> lock(connections_info_mut_);
    if (connections_info_.find(reinterpret_cast<ptrdiff_t>(connectionInfo)) != connections_info_.end())
        connections_info_.erase(reinterpret_cast<ptrdiff_t>(connectionInfo));
}


void Server::SendPacket(Packet& packet)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    std::map<ptrdiff_t, Connection*>::iterator it = connections_.begin();
    std::map<ptrdiff_t, Connection*>::iterator ite= connections_.end();
    
    for (; it != ite; ++it)
        it->second->AddPacket(packet);
}

void Server::SendMetric(Metric & metric)
{
	Packet packet;
	packet << static_cast<uint32_t>(metric.wolfNum);
	packet << metric.wolfMoy;
	packet << static_cast<uint32_t>(metric.wolfActionNum);
	packet << static_cast<uint32_t>(metric.wolfActionNeural);
	packet << static_cast<uint32_t>(metric.sheepNum);
	packet << metric.sheepMoy;
	packet << static_cast<uint32_t>(metric.sheepActionNum);
	packet << static_cast<uint32_t>(metric.sheepActionNeural);
	boost::lock_guard<boost::mutex> lock(connections_info_mut_);
    std::map<ptrdiff_t, ConnectionInfo*>::iterator it = connections_info_.begin();
    std::map<ptrdiff_t, ConnectionInfo*>::iterator ite= connections_info_.end();
    
    for (; it != ite; ++it)
        it->second->AddPacket(packet);
}

void Server::synchronize(Connection * connection)
{
	std::cout << "Synchronize..." << std::endl;
	Packet packet;
	Logique::Board board;

	boost::lock_guard<boost::mutex> lock(connections_mut_);
	connection->Lock();
	
	Logique::Callback_Environnement::EventProxy lockguard = environnement_.getEventProxy();
	environnement_.getBoard(board);

	packet << static_cast<uint8_t>(Logique::Environnement_Event::NONE) << static_cast<uint32_t>(Logique::BOARD_SIZE);
	connection->AddSynchroPacket(packet);

	unsigned int numSheep = 0;
	unsigned int numWolf = 0;
	for (uint32_t x = 0; x < Logique::BOARD_SIZE; x++)
	{
		for (uint32_t y = 0; y < Logique::BOARD_SIZE; y++)
		{
			ptrdiff_t e = 0;
			Packet grass;
			Packet entity;
			Logique::Square sq = board.get(x, y);

			e = sq.getId(Logique::Square::SHEEP);
			if (e != 0)
			{
				numSheep++;
				entity << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_SPAWN) << e;
				entity << Logique::Square::SHEEP << static_cast<int32_t>(x) << static_cast<int32_t>(y);
				connection->AddSynchroPacket(entity);
			}

			e = sq.getId(Logique::Square::WOLF);

			if (e != 0)
			{
				numWolf++;
				entity << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_SPAWN) << e;
				entity << Logique::Square::WOLF << static_cast<int32_t>(x) << static_cast<int32_t>(y);
				connection->AddSynchroPacket(entity);
			}

			if (sq.hasGrass())
			{
				grass << static_cast<uint8_t>(Logique::Environnement_Event::GRASS_UP) << static_cast<int32_t>(x) << static_cast<int32_t>(y);
				connection->AddSynchroPacket(grass);
			}
		}
	}
	connection->Unlock();

	std::cout << "numSheep:" << numSheep << " numWolf:" << numWolf << std::endl;
	std::cout << "...end" << std::endl;
}

void Server::forgePacket(Packet & packet, Logique::Environnement_Event & e)
{
	if (e._type > Logique::Environnement_Event::NONE && e._type < Logique::Environnement_Event::TYPE_SIZE)
		forge_[e._type](packet, e);
}

void Server::spawnForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_SPAWN) << e._entityId << e._entityType << e._pos.x << e._pos.y;
}

void Server::moveForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_MOVE) << e._entityId << e._newPos.x << e._newPos.y;
}

void Server::eatForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_EAT) << e._entityId;
}

void Server::dieForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_DEATH) << e._entityId;
}

void Server::reproduceForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::ENTITY_REPRODUCE) << e._entityId;
}

void Server::grassUpForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::GRASS_UP) << e._pos.x << e._pos.y;
}

void Server::grassDownForge(Packet & output, Logique::Environnement_Event &e)
{
	output << static_cast<uint8_t>(Logique::Environnement_Event::GRASS_DOWN) << e._pos.x << e._pos.y;
}

void Server::run()
{
	//boost::xtime xt;
	//boost::xtime_get(&xt, boost::TIME_UTC);
	//xt.sec += 5;
	while (true)
	{
		//boost::thread::sleep(xt);
		Logique::Callback_Environnement::EventProxy proxy = environnement_.getEventProxy();
		Logique::Callback_Environnement::EventDeque::iterator it = proxy.begin();
		Logique::Callback_Environnement::EventDeque::iterator ite = proxy.end();
		for (;it != ite && connections_.size() > 0; ++it)
		{
			Packet packet;
			forgePacket(packet, (*it));
			SendPacket(packet);
		}
		proxy.clear();
		Logique::Callback_Environnement::MetricProxy mProxy = environnement_.getMetricProxy();
		Logique::Callback_Environnement::MetricDeque::iterator mit = mProxy.begin();
		Logique::Callback_Environnement::MetricDeque::iterator mite = mProxy.end();
		for (; mit != mite; ++mit)
		{
			SendMetric((*mit));
		}
		mProxy.clear();
		//xt.sec += 2;
	}
}