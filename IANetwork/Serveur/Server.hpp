#ifndef _A_SERVER_HPP_
#define _A_SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <stdint.h>
#include <string>
#include <vector>
#include <deque>
#include "Package.hpp"

using boost::asio::ip::tcp;

class Server
{
public:
	typedef boost::shared_ptr<tcp::socket>	socket_ptr;
	typedef std::vector<socket_ptr>			socket_container;
	typedef std::deque<Package> Package_queue;	
	typedef boost::shared_ptr<Package>	BufferedPackage;

private:
	tcp::acceptor acceptor_;
	socket_container sockets_;
	socket_container sockets_to_disc_;
	Package_queue	incomming_packages_;
	bool			run_;
	boost::mutex	packages_mutex_;
	boost::mutex	sockets_mutex_;

public:
	Server(boost::asio::io_service & io_service, boost::asio::ip::tcp & protocol, int32_t port);
	virtual ~Server(void);

	void run();
	virtual void disconnected(socket_ptr & socket);

private:
	void start();
	void stop();
	void handle_accept(socket_ptr & socket, boost::system::error_code const & error);
	void set_handle_read(socket_ptr & socket, boost::system::error_code const & error);
	void handle_write(socket_ptr & socket, boost::system::error_code const & error);
	void handle_incomming_package(BufferedPackage & package, socket_ptr & socket, boost::system::error_code const & error);
	bool send_message(socket_ptr & socket, BufferedPackage & message);
};

#endif