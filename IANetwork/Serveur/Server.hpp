#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include "Package.hpp"

namespace Networking
{

	using boost::asio::ip::tcp;

	class Server
	{
	public:
		typedef boost::shared_ptr<tcp::socket>	socket_ptr;
		typedef boost::shared_ptr<Package>	Package_ptr;
		typedef std::map<tcp::socket::native_handle_type, socket_ptr>			socket_container;
		typedef std::vector<tcp::socket::native_handle_type>	mark_container;	
		typedef std::deque<Package_ptr> Package_queue;	
		typedef boost::function<void (void)> func_t;

	private:
		tcp::acceptor acceptor_;
		socket_container sockets_;
		mark_container	 marks_;
		Package_queue	incomming_packages_;
		Package_queue	sending_packages_;
		bool			run_;
		boost::mutex	in_packages_mut_;
		boost::mutex	out_packages_mut_;
		boost::mutex	sockets_mut_;
		func_t synchronize_;

	public:
		Server(boost::asio::io_service & io_service, boost::asio::ip::tcp & protocol, int32_t port);
		virtual ~Server(void);

		void run();
		void add_sending_package(Package_ptr & package);
		void add_sending_packages(Package_queue & packages);
		void setSynchronize(func_t const & synchronize);

	private:
		void start();
		void stop();
		void disconnected(socket_ptr & socket);
		void cleaning_sockets();

		void handle_accept(socket_ptr & socket, boost::system::error_code const & error);
		void set_handle_read(socket_ptr & socket, boost::system::error_code const & error);
		void handle_incomming_package(Package_ptr & package, socket_ptr & socket, boost::system::error_code const & error);
		bool send_message(socket_ptr & socket, Package_ptr & message);
	};

}

#endif