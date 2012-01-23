/* 
 * File:   Connection.hpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on January 17, 2012, 11:53 AM
 */

#ifndef CONNECTION_HPP
#define	CONNECTION_HPP

#include <Poco/Net/TCPServerConnection.h>
#include <boost/thread.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <deque>
#include "Packet.hpp"

class Connection : public Poco::Net::TCPServerConnection {
public:
    Connection(const Poco::Net::StreamSocket & socket);
    virtual ~Connection();

	void run();
	void Lock();
	void AddSynchroPacket(Packet & packet);
	void Unlock();
    void AddPacket(Packet & packet);

private:

    std::deque<Packet, boost::pool_allocator<Packet> > packets_;
    boost::mutex packet_mut_;
};

#endif	/* CONNECTION_HPP */

