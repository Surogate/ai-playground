#ifndef _WOLF_HPP_
#define _WOLF_HPP_

#include "Entity.hpp"

class Wolf 
	: public Entity
{
public:
	Wolf(void);
	~Wolf(void);

	void move(World & world);
	Server::BufferedPackage serialize();
};

#endif
