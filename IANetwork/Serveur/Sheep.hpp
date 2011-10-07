#ifndef _SHEEP_HPP_
#define _SHEEP_HPP_

#include "Entity.hpp"

class Sheep 
	: public Entity
{
public:
	Sheep(void);
	~Sheep(void);

	void move(World & world);
	Server::BufferedPackage serialize(); 
};

#endif