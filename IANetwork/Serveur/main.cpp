#include <iostream>
#include <ctime>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <stdint.h>
#include "Server.hpp"

using boost::asio::ip::tcp;

int main(int ac, char **av)
{
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp version = tcp::v4();
		Networking::Server server(io_service, version, (int32_t)16000);
		boost::thread service_thr(boost::bind(&boost::asio::io_service::run, &io_service));
		server.run();
		service_thr.join();
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
	}
	return (EXIT_SUCCESS);
}
