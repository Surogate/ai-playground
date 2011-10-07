#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "Entity.hpp"
#include "Server.hpp"
#include <boost/shared_ptr.hpp>
#include <vector>
#include <stdint.h>

class World
{
public:
	typedef boost::shared_ptr<Entity> Entity_ptr;
	typedef std::vector<Entity_ptr> Entities;

private:
	Entities	entities_;
	int32_t		width_;
	int32_t		height_;

public:
	World(void);
	~World(void);

	void update();
	int32_t get_width();
	int32_t get_height();
	Entities & get_entities();
	Networking::Server::BufferedPackage serialize();
};

#endif