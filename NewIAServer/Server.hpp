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
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <map>
#include <stdint.h>
#include "Packet.hpp"
#include "Connection.hpp"
#include "Callback_Environnement.hpp"
#include "Environnement.hpp"
#include "Singleton.hpp"

class Server : public Logique::Singleton<Server> {
    friend class Logique::Singleton<Server>;

public:
    void Start();
    void SendPacket(Packet & packet);
    void AddConnection(Connection * connection);
    void RemoveConnection(Connection * connection);
	void synchronize(Connection * connection);

private:
    Server();
    virtual ~Server();

    Poco::Net::ServerSocket				socket_;
    Poco::Net::TCPServer				server_;
    std::map<ptrdiff_t, Connection*>	connections_;
    Logique::Environnement				environnement_;
    boost::mutex						connections_mut_;
	boost::function<void (Packet &, Logique::Environnement_Event &)>	forge_[Logique::Environnement_Event::TYPE_SIZE];

	Packet * forgePacket(Logique::Environnement_Event & e);
	void spawnForge(Packet & output, Logique::Environnement_Event & e);
	void moveForge(Packet & output, Logique::Environnement_Event & e);
	void eatForge(Packet & output, Logique::Environnement_Event & e);
	void dieForge(Packet & output, Logique::Environnement_Event & e);
	void reproduceForge(Packet & output, Logique::Environnement_Event & e);
	void grassUpForge(Packet & output, Logique::Environnement_Event & e);
	void grassDownForge(Packet & output, Logique::Environnement_Event & e);
	void run();
};

#endif	/* SERVER_HPP */

