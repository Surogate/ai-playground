/* 
 * File:   Server.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 17, 2012, 4:54 PM
 */

#ifndef SERVER_HPP
#define	SERVER_HPP

#include <Poco/Net/TCPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <boost/thread/mutex.hpp>
#include <map>
#include <stdint.h>
#include "Packet.hpp"
#include "Connection.hpp"
#include "Environnement.hpp"
#include "Singleton.hpp"

class Server : public Logique::Singleton<Server> {
    friend class Logique::Singleton<Server>;

    static const uint8_t SPAWN;
    static const uint8_t MOVE;
    static const uint8_t EAT;
    static const uint8_t DIE;
    static const uint8_t REPRODUCE;
    static const uint8_t BOARD_BEG;
    static const uint8_t BOARD;
    static const uint8_t BOARD_END;
    
public:
    void Start();
    void SendPacket(Packet & packet);
    void AddConnection(Connection * connection);
    void RemoveConnection(Connection * connection);
	void synchronize(Connection * connection);

private:
    Server();
    virtual ~Server();

    Poco::Net::ServerSocket socket_;
    Poco::Net::TCPServer server_;
    std::map<std::size_t, Connection*> connections_;
    Logique::Environnement         environnement_;
    boost::mutex				   connections_mut_;

    void cmdSpawnSheep(Logique::Entity const &);
    void cmdSpawnWolf(Logique::Entity const &);
    void cmdEntityMove(Logique::Entity const &);
    void cmdEntityEat(Logique::Entity const &);
    void cmdEntityDead(Logique::Entity const &);
    void cmdReproduce(Logique::Entity const &);
    void cmdBoardChange(Logique::Board const &);
};

#endif	/* SERVER_HPP */

