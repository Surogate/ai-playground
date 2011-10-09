#include "Sheep.hpp"
#include "World.hpp"
#include <cmath>
#include <sstream>

#include "Package.hpp"

Sheep::Sheep(void) : Entity()
{
}


Sheep::~Sheep(void)
{
}

void Sheep::move(World & world)
{
	x_ = std::rand() % world.get_width();
	y_ = std::rand() % world.get_height();
}

Networking::Server::BufferedPackage Sheep::serialize()
{
	std::stringstream ss;
	std::string	s;

	ss << "s:" << id_ << ":" << x_ << ":" << y_ << "eof";
	ss >> s;
	Networking::Server::BufferedPackage pack = Networking::Server::BufferedPackage(new Package());
	pack->init_from_string(s);
	return (pack);
}
