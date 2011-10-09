#include "Entity.hpp"

uint32_t Entity::gl_entity_id = 0;

Entity::Entity(void)
{
	gl_entity_id++;
	id_ = gl_entity_id;
	x_ = 0;
	y_ = 0;
}


Entity::~Entity(void)
{
}
