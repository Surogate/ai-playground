#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <string>
#include <stdint.h>

#include "Server.hpp"

class World;

class Entity
{
public:
	static uint32_t gl_entity_id;

protected:
	uint32_t id_;
	int32_t x_;
	int32_t y_;

public:
	Entity(void);
	virtual ~Entity(void);

	virtual void move(World & world) = 0;
	virtual Server::BufferedPackage serialize() = 0; 
};

#endif
