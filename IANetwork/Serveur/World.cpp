#include "World.hpp"
#include "Wolf.hpp"
#include "Sheep.hpp"
#include <sstream>
#include <cmath>
#include <ctime>

World::World(void)
{
	width_ = 5;
	height_ = 5;
	entities_.push_back(Entity_ptr(new Wolf()));
	entities_.push_back(Entity_ptr(new Wolf()));
	entities_.push_back(Entity_ptr(new Wolf()));
	entities_.push_back(Entity_ptr(new Sheep()));
	entities_.push_back(Entity_ptr(new Sheep()));
	entities_.push_back(Entity_ptr(new Sheep()));
	
	std::srand(std::time(0));
}


World::~World(void)
{
}

void World::update()
{
	for (uint32_t i = 0; i < entities_.size(); i++)
	{
		entities_[i]->move(*this);
	}
}

int32_t World::get_height()
{
	return (height_);
}

int32_t World::get_width()
{
	return (width_);
}

World::Entities & World::get_entities()
{
	return (entities_);
}

Networking::Server::BufferedPackage World::serialize()
{
	std::stringstream ss;
	std::string	s;

	ss << "world:"<< width_ << ":" << height_;
	ss >> s;
	Networking::Server::BufferedPackage pack = Networking::Server::BufferedPackage(new Package());
	pack->init_from_string(s);
	return (pack);
}
