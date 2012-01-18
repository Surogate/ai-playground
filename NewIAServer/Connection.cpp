/* 
 * File:   Connection.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 11:53 AM
 */

#include "Connection.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>

Connection::Connection(const Poco::Net::StreamSocket & socket) :
Poco::Net::TCPServerConnection(socket) {
    Server::getInstance().AddConnection(this);
}

Connection::~Connection() {
    std::cout << "Disconnection from : " << socket().address().toString() << std::endl;
}

void Connection::run() {
    std::cout << "Connection from : " << socket().address().toString() << std::endl;
    char buffer[512];
    while (true)
    {
        if (socket().poll(Poco::Timespan(1000), Poco::Net::Socket::SELECT_READ))
        {
            if (socket().receiveBytes(buffer, 512, 0) < 0)
                return ;
            // TODO : interpret this.
        }
        else
        {
            boost::lock_guard<boost::mutex> lock(packet_mut_);
            while (!packets_.empty())
            {
                Packet & pack = packets_.back();
                socket().sendBytes(pack.GetData(), pack.GetSize(), 0);
                packets_.pop_back();
            }
        }
    }
}

void Connection::AddPacket(Packet & packet)
{
    boost::lock_guard<boost::mutex> lock(packet_mut_);
    packets_.push_back(packet);
}

