#include "Server.hpp"
#include <ctime>
#include <iostream>
#include <boost/bind.hpp>

namespace Networking
{

	Server::Server(boost::asio::io_service & io_service, boost::asio::ip::tcp & protocol, int32_t port)
		: acceptor_(io_service, tcp::endpoint(protocol, port))
	{
	}

	Server::~Server(void)
	{
	}

	void Server::start()
	{
		socket_ptr new_connection(new tcp::socket(acceptor_.get_io_service()));
		acceptor_.async_accept(*new_connection,
			boost::bind(&Server::handle_accept, this, new_connection, boost::asio::placeholders::error));
	}

	void Server::stop()
	{
		acceptor_.get_io_service().stop();
	}

	void Server::handle_accept(socket_ptr & socket, boost::system::error_code const & error)
	{
		if (!error)
		{
			boost::lock_guard<boost::mutex> lock(sockets_mut_);
			std::clog << "[LOG] new connection from : " << socket->remote_endpoint().address().to_string() << std::endl;
			sockets_[socket->native_handle()] = socket;
			set_handle_read(socket, error);
		}
		else
		{
			std::cerr << "Error : " << error.message() << std::endl;
		}
		start();
	}

	void Server::disconnected(socket_ptr & socket)
	{
		boost::lock_guard<boost::mutex> lock(sockets_mut_);
		
		mark_container::iterator it = std::find(marks_.begin(), marks_.end(), socket->native_handle());
		if (it == marks_.end())
			marks_.push_back(socket->native_handle());
	}

	void Server::run()
	{
		this->start();
		while (run_)
		{
			boost::lock_guard<boost::mutex> lk_in_package(in_packages_mut_);
			while (!incomming_packages_.empty())
			{
				Package_ptr & pack = incomming_packages_.back();
				// Interpretation des commandes a distance.
				incomming_packages_.pop_back();
			}
			boost::lock_guard<boost::mutex> lk_out_package(out_packages_mut_);
			while (!sending_packages_.empty())
			{
				//socket_container::iterator it = sockets_.begin();
				//socket_container::iterator ite = sockets_.end();
				//for (; it != ite; ++it)
				//{
				//	if ((it->second)->is_open())
				//	{
				//		for (uint32_t y = 0; y < world.get_entities().size(); y++)
				//		{
				//			if (!send_message((it->second), /*msg*/))
				//				break;
				//		}
				//	}
				//}
			}
			this->cleaning_sockets();
		}
		this->stop();
	}

	void Server::add_sending_package(Package_ptr & package)
	{
		boost::lock_guard<boost::mutex> lk(out_packages_mut_);
		sending_packages_.push_back(package);
	}

	void Server::set_handle_read(socket_ptr & socket, boost::system::error_code const & error)
	{
		if (!error)
		{
			Package_ptr new_package = Package_ptr(new Package());
			boost::asio::async_read(*socket,
				boost::asio::buffer(new_package->get_buffer()),
				boost::bind(&Server::handle_incomming_package, this, new_package, socket, boost::asio::placeholders::error));
		}
		else
		{
			disconnected(socket);
		}
	}


	void Server::handle_incomming_package(Package_ptr & package, socket_ptr & socket, boost::system::error_code const & error)
	{
		if (!error)
		{
			std::clog << "[LOG] Incomming package from : " << socket->remote_endpoint().address().to_string() << std::endl;
			boost::lock_guard<boost::mutex> lock(in_packages_mut_);
			incomming_packages_.push_back(package);
			set_handle_read(socket, error);
		}
		else
		{
			disconnected(socket);
		}
	}

	bool Server::send_message(socket_ptr & socket, Package_ptr & message)
	{
		boost::system::error_code error;
		boost::asio::write(*socket,
			boost::asio::buffer(message->get_buffer().data(), message->size()),
			error);
		if (error)
		{
			disconnected(socket);
			return false;
		}
		return true;
	}

	void Server::cleaning_sockets()
	{
		boost::lock_guard<boost::mutex> lock(sockets_mut_);
		mark_container::iterator it = marks_.begin();
		mark_container::iterator ite = marks_.end();

		for (;it != ite; ++it)
		{
			// Check if the key exist in the sockets
			socket_container::iterator its = sockets_.find(*it);
			if (its != sockets_.end())
			{	
				std::clog << "[LOG] disconnection from : " << its->second->remote_endpoint().address().to_string() << std::endl;
				sockets_.erase((*it));
			}
		}
		marks_.clear();
	}
}