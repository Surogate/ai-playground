#include "Wolf.hpp"
#include "World.hpp"
#include <sstream>

#include "Package.hpp"


Wolf::Wolf(void) : Entity()
{
}


Wolf::~Wolf(void)
{
}

void Wolf::move(World & world)
{
	x_ = std::rand() % world.get_width();
	y_ = std::rand() % world.get_height();
}

Server::BufferedPackage Wolf::serialize()
{
	std::stringstream ss;
	std::string	s;

	ss << "w:" << id_ << ":" << x_ << ":" << y_ << "eof";
	ss >> s;
	Server::BufferedPackage pack = Server::BufferedPackage(new Package());
	pack->init_from_string(s);
	return (pack);
}