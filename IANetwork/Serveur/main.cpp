#include <iostream>
#include <ctime>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "Server.hpp"

using boost::asio::ip::tcp;

int main(int ac, char **av)
{
	try
	{
		boost::asio::io_service io_service;
		Networking::Server server(io_service, tcp::v4(), 16000);
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