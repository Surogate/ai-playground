#include "Server.hpp"
#include "World.hpp"
#include <ctime>
#include <iostream>
#include <boost/bind.hpp>

Server::Server(boost::asio::io_service & io_service, boost::asio::ip::tcp & protocol, int32_t port)
	: acceptor_(io_service, tcp::endpoint(protocol, port)), sockets_(0)
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
		boost::lock_guard<boost::mutex> lock(sockets_mutex_);
		std::clog << "[LOG] new connection from : " << socket->remote_endpoint().address().to_string() << std::endl;
		sockets_.push_back(socket);
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
	boost::lock_guard<boost::mutex> lock(sockets_mutex_);
	socket_container::iterator it = sockets_.begin();
	socket_container::iterator ite = sockets_.end();

	for (; it != ite; ++it)
	{
		if ((*it) == socket)
		{
			std::clog << "[LOG] disconnection from : " << socket->remote_endpoint().address().to_string() << std::endl;
			sockets_.erase(it);
			socket->close();
			return ;
		}
	}
}

void Server::run()
{
	this->start();
	World world;
	while (run_)
	{
		boost::lock_guard<boost::mutex> lock_package(packages_mutex_);
		while (!incomming_packages_.empty())
		{
			Package pack = incomming_packages_.back();
			// Interpretation des commandes a distance.
			incomming_packages_.pop_back();
		}
		world.update();
		for (uint32_t i = 0; i < sockets_.size(); i++)
		{
			if (sockets_[i]->is_open())
			{
				for (uint32_t y = 0; y < world.get_entities().size(); y++)
				{
					if (!send_message(sockets_[i], world.get_entities()[y]->serialize()))
						break;
				}
			}
		}
	}
	this->stop();
}

void Server::set_handle_read(socket_ptr & socket, boost::system::error_code const & error)
{
	if (!error)
	{
		BufferedPackage new_package = BufferedPackage(new Package());
		boost::asio::async_read(*socket,
			boost::asio::buffer(new_package->get_buffer()),
			boost::bind(&Server::handle_incomming_package, this, new_package, socket, boost::asio::placeholders::error));
	}
	else
	{
		disconnected(socket);
	}
}


void Server::handle_incomming_package(BufferedPackage & package, socket_ptr & socket, boost::system::error_code const & error)
{
	if (!error)
	{
		std::clog << "[LOG] Incomming package from : " << socket->remote_endpoint().address().to_string() << std::endl;
		boost::lock_guard<boost::mutex> lock(packages_mutex_);
		incomming_packages_.push_back(Package(*package));
		set_handle_read(socket, error);
	}
	else
	{
		disconnected(socket);
	}
}

void Server::handle_write(socket_ptr & socket, boost::system::error_code const & error)
{
	if (error)
	{
		//std::cerr << "[Error] : " << error.message() << std::endl;
		disconnected(socket);
	}
}

bool Server::send_message(socket_ptr & socket, BufferedPackage & message)
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
