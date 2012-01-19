
#ifndef ENVIRONNEMENT_EVENT_HPP
#define ENVIRONNEMENT_EVENT_HPP

#include "Coord.hpp"
#include "Square.hpp"

class Entity;

namespace Logique {

	struct Environnement_Event {
		enum Type {
			NONE,
			ENTITY_SPAWN,
			ENTITY_MOVE,
			ENTITY_REPRODUCE,
			ENTITY_EAT,
			ENTITY_DEATH,
			GRASS_UP,
			GRASS_DOWN
		};

		Type _type;
		int _entityId;
		int _entityType;
		Coord _pos;
		Coord _newPos;

		Environnement_Event(Type type, Entity& id, Square::EntityContain value, Coord pos, Coord newpos)
			: _type(type), _entityId(reinterpret_cast<int>(&id)), _entityType(value), _pos(pos), _newPos(newpos)
		{}

		Environnement_Event()
			: _type(NONE), _entityId(0), _entityType(), _pos(), _newPos()
		{}

		Environnement_Event(Type type, Entity& id, Square::EntityContain value, Coord pos)
			: _type(type), _entityId(reinterpret_cast<int>(&id)), _entityType(value), _pos(pos), _newPos(pos)
		{}

		Environnement_Event(Type type, Coord pos)
			: _type(type), _entityId(0), _pos(pos), _entityType(-1), _newPos(pos)
		{}
	};

}

#endif