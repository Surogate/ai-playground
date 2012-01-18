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

const uint32_t Connection::BUFF_SIZE = 64U;

Connection::Connection(const Poco::Net::StreamSocket & socket) :
Poco::Net::TCPServerConnection(socket) {
    Server::getInstance().AddConnection(this);
}

Connection::~Connection() {
    std::cout << "Disconnection from : " << socket().peerAddress().toString() << std::endl;
	Server::getInstance().RemoveConnection(this);
}

void Connection::run() {
	std::cout << "Connection from : " << socket().peerAddress().toString() << std::endl;
	Server::getInstance().synchronize(this);
	char buffer[512];
	try
	{
		while (true)
		{
			boost::lock_guard<boost::mutex> lock(packet_mut_);

			if (socket().poll(Poco::Timespan(1000), Poco::Net::Socket::SELECT_READ))
			{
				if (socket().receiveBytes(buffer, 512, 0) <= 0)
					return ;
				// TODO : interpret this.
			}
			else
			{
				while (!packets_.empty())
				{
					Packet & pack = packets_.back();
					while (pack.GetSize() < Connection::BUFF_SIZE)
						pack << (uint8_t)0;
					socket().sendBytes(pack.GetData(), pack.GetSize(), 0);
					packets_.pop_back();
				}
			}
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Connection::AddPacket(Packet & packet)
{
    boost::lock_guard<boost::mutex> lock(packet_mut_);
    packets_.push_back(packet);
}

