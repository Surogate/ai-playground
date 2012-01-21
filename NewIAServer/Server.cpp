/* 
 * File:   Server.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 4:54 PM
 */

#include "Server.hpp"
#include <iostream>

const uint8_t Server::SPAWN     = 0;
const uint8_t Server::MOVE      = 1;
const uint8_t Server::EAT       = 2;
const uint8_t Server::DIE       = 3;
const uint8_t Server::REPRODUCE = 4;
const uint8_t Server::BOARD_BEG = 5;
const uint8_t Server::BOARD     = 6;
const uint8_t Server::BOARD_END = 7;

Server::Server() :
        socket_(16000),
        server_(new Poco::Net::TCPServerConnectionFactoryImpl<Connection>(),
        socket_),
        connections_(),
        environnement_(),
		connections_mut_() {

}

Server::~Server() {
}


void Server::Start()
{
    std::cout << "server start" << std::endl;
    server_.start();
    environnement_.run();
}

void Server::AddConnection(Connection* connection)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    connections_[(std::size_t)connection] = connection; 
	//synchronize(connection);
}

void Server::RemoveConnection(Connection* connection)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    if (connections_.find((std::size_t)connection) != connections_.end())
        connections_.erase((std::size_t)connection);
}

void Server::SendPacket(Packet& packet)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    std::map<std::size_t, Connection*>::iterator it = connections_.begin();
    std::map<std::size_t, Connection*>::iterator ite = connections_.end();
    
    for (; it != ite; ++it)
        it->second->AddPacket(packet);
}

void Server::synchronize(Connection * connection)
{
	boost::lock_guard<boost::mutex> lock(connections_mut_);
    std::clog << "[Log] synchronize... entity..." << std::endl;
	environnement_.lock();
	Logique::Environnement::EntityPtrSet::const_iterator it = environnement_.getEntityList().begin();
	Logique::Environnement::EntityPtrSet::const_iterator ite = environnement_.getEntityList().end();
	for (; it != ite; ++it)
	{
		Packet packet;
		packet << Server::SPAWN << (std::size_t) it->first << (uint8_t)it->first->getType() << (int32_t)it->first->getLocation().x << (int32_t)it->first->getLocation().y;
		connection->AddPacket(packet);
	}
	environnement_.unlock();
	std::clog << "[Log] synchronize finished..." << std::endl;
}

void Server::cmdSpawnSheep(Logique::Entity const &e)
{
    Packet packet;

    packet << Server::SPAWN << (std::size_t) &e << (uint8_t)e.getType() << (int32_t)e.getLocation().x << (int32_t)e.getLocation().y;
    SendPacket(packet);
}

void Server::cmdSpawnWolf(Logique::Entity const &e)
{
    Packet packet;
    
    packet << Server::SPAWN << (std::size_t) &e << (uint8_t)e.getType() << (int32_t)e.getLocation().x << (int32_t)e.getLocation().y;
    SendPacket(packet);
}

void Server::cmdEntityMove(Logique::Entity const &e)
{
     Packet packet;

    packet << Server::MOVE << (std::size_t) &e << (uint8_t)e.getLastAction() << (int32_t)e.getLocation().x << (int32_t)e.getLocation().y;
    SendPacket(packet);
}

void Server::cmdEntityEat(Logique::Entity const &e)
{
    Packet packet;

    packet << Server::EAT << (int32_t)e.getLocation().x << (int32_t)e.getLocation().y;
    SendPacket(packet);
}

void Server::cmdEntityDead(Logique::Entity const &e)
{
    Packet packet;

    packet << Server::DIE << (std::size_t) &e;
    SendPacket(packet);
}

void Server::cmdReproduce(Logique::Entity const &e)
{
    Packet packet;

    packet << Server::REPRODUCE << (std::size_t) &e;
    SendPacket(packet);
}

void Server::cmdBoardChange(Logique::Board const &board)
{
	const_cast<Logique::Board&>(board).lock();

	Packet pBegin;

	pBegin << Server::BOARD_BEG << (uint32_t)board.size();
	SendPacket(pBegin);
	for (uint32_t x = 0; x < board.size(); x++)
	{
		for (uint32_t y = 0; y < board.size(); y++)
		{
			Packet pBoard;

			pBoard << Server::BOARD << x << y << (uint8_t)board.get(Coord(x, y)).hasGrass() << (uint32_t)board.get(Coord(x, y)).odour();
			SendPacket(pBoard);
		}
	}
	Packet pEnd;

	pEnd << Server::BOARD_END;
	SendPacket(pEnd);
	const_cast<Logique::Board&>(board).unlock();
}