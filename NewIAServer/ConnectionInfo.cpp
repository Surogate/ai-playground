/* 
 * File:   ConnectionInfo.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 * 
 * Created on January 17, 2012, 11:53 AM
 */

#include "ConnectionInfo.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>


ConnectionInfo::ConnectionInfo(const Poco::Net::StreamSocket & socket) :
Poco::Net::TCPServerConnection(socket) {
    Server::getInstance().AddConnectionInfo(this);
}

ConnectionInfo::~ConnectionInfo() {
    std::cout << "Disconnection Info from : " << socket().peerAddress().toString() << std::endl;
	Server::getInstance().RemoveConnectionInfo(this);
}

void ConnectionInfo::run() {
	std::cout << "Connection Info from : " << socket().peerAddress().toString() << std::endl;
	char buffer[512];
	try
	{
		while (true)
		{
			if (socket().poll(Poco::Timespan(1000), Poco::Net::Socket::SELECT_READ))
			{
				if (socket().receiveBytes(buffer, 512, 0) <= 0)
					return ;
				// TODO : interpret this.
			}
			else
			{
				boost::lock_guard<boost::mutex> lock(packet_mut_);
				while (!packets_.empty())
				{
					Packet & pack = packets_.front();
					std::size_t size = pack.Endianl(pack.GetSize());
					socket().sendBytes(reinterpret_cast<const void*>(&size), sizeof(size));
					std::size_t send = 0;
					for (; send < pack.GetSize(); )
						send = socket().sendBytes(pack.GetData() + send, pack.GetSize() - send, 0);
					packets_.pop_front();
				}
			}
		}
	}
	catch (std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void ConnectionInfo::AddPacket(Packet & packet)
{
    boost::lock_guard<boost::mutex> lock(packet_mut_);
    packets_.push_back(packet);
}

void ConnectionInfo::AddSynchroPacket(Packet & packet)
{
	packets_.push_back(packet);
}

void ConnectionInfo::Lock()
{
	packet_mut_.lock();
}

void ConnectionInfo::Unlock()
{
	packet_mut_.unlock();
}
