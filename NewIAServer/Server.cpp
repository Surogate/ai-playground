/* 
 * File:   Server.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 4:54 PM
 */

#include "Server.hpp"
#include <iostream>

const uint8_t Server::SPAWN     = 1;
const uint8_t Server::MOVE      = 2;
const uint8_t Server::EAT       = 3;
const uint8_t Server::DIE       = 4;
const uint8_t Server::REPRODUCE = 5;
const uint8_t Server::BOARD_BEG = 6;
const uint8_t Server::BOARD     = 7;
const uint8_t Server::BOARD_END = 8;

Server::Server() :
        socket_(16000),
        server_(new Poco::Net::TCPServerConnectionFactoryImpl<Connection>(),
        socket_),
        connections_(),
        environnement_() {
    std::cout << "socket : " << socket_.address().toString() << std::endl;
    // Still working don't do any change.
    environnement_.setSpawnSheep(boost::bind(&Server::cmdSpawnSheep, this, _1));
    environnement_.setSpawnWolf(boost::bind(&Server::cmdSpawnWolf, this, _1));
    environnement_.setOnEntityMove(boost::bind(&Server::cmdEntityMove, this, _1));
    environnement_.setOnReproduce(boost::bind(&Server::cmdReproduce, this, _1));
    environnement_.setOnEntityEat(boost::bind(&Server::cmdEntityEat, this, _1));
    environnement_.setOnEntityDead(boost::bind(&Server::cmdEntityDead, this, _1));
    environnement_.setOnBoardChange(boost::bind(&Server::cmdBoardChange, this, _1));
}

Server::~Server() {
}


void Server::Start()
{
    std::cout << "server start" << std::endl;
    server_.start();
//    environnement_.run();
}

void Server::AddConnection(Connection* connection)
{
    connections_[(std::size_t)connection] = connection; 
}

void Server::RemoveConnection(Connection* connection)
{
    if (connections_.find((std::size_t)connection) != connections_.end())
        connections_.erase((std::size_t)connection);
}

void Server::SendPacket(Packet& packet)
{
    std::map<std::size_t, Connection*>::iterator it = connections_.begin();
    std::map<std::size_t, Connection*>::iterator ite = connections_.end();
    
    for (; it != ite; ++it)
        it->second->AddPacket(packet);
}

void Server::synchronize(Connection * connection)
{
    
}

void Server::cmdSpawnSheep(Logique::Entity const &e)
{
    Packet packet;
    
    packet << Server::SPAWN << e.getType() << e.getLocation().x << e.getLocation().y;
    SendPacket(packet);
}

void Server::cmdSpawnWolf(Logique::Entity const &e)
{
    Packet packet;
    
    packet << Server::SPAWN << e.getType() << e.getLocation().x << e.getLocation().y;
    SendPacket(packet);    
}

void Server::cmdEntityMove(Logique::Entity const &e)
{
    
}

void Server::cmdEntityEat(Logique::Entity const &e)
{
    
}

void Server::cmdEntityDead(Logique::Entity const &e)
{
    
}

void Server::cmdReproduce(Logique::Entity const &e)
{
    
}

void Server::cmdBoardChange(Logique::Board const &board)
{
    
}