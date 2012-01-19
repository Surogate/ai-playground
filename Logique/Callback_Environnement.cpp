
#include "Callback_Environnement.hpp"

namespace Logique {

	Callback_Environnement::Callback_Environnement() 
	{
		initEventTypeString();
		initEntityTypeString();
	}

	Callback_Environnement::~Callback_Environnement() {}

	Callback_Environnement::Callback_Environnement(const Callback_Environnement& orig) {}

	void Callback_Environnement::addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos, Coord newPos)
	{
		//debugEvent(value, type, pos, newPos);
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, id, type, pos, newPos));
		_mut.unlock();
	}

	void Callback_Environnement::addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos)
	{
		//debugEvent(value, type, pos);
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, id, type, pos));
		_mut.unlock();
	}
	void Callback_Environnement::addAction(Environnement_Event::Type value, Coord pos)
	{
		//debugEvent(value, pos);
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, pos));
		_mut.unlock();
	}

	EventProxy&& Callback_Environnement::getEventProxy() {
		return DequeProxy< Environnement_Event >(_eventQueue, _mut);
	}

	void Callback_Environnement::debugEvent(const Environnement_Event& ev) 
	{
		if (ev._entityId > 0) 
		{
			std::cout << _eventTypeString[ev._type] << " id:" << ev._entityId << " " << _entityTypeString[ev._entityType] << " " << ev._pos;
			if (ev._pos != ev._newPos)
				std::cout << " " << ev._newPos << std::endl;
			else 
				std::cout << std::endl;
		} else {
			std::cout << _eventTypeString[ev._type] << " " << ev._pos << std::endl;
		}
	}
	
	void Callback_Environnement::debugEvent(Environnement_Event::Type value, Square::EntityContain type, Coord pos)
	{
		std::cout << _eventTypeString[value] << " " << _entityTypeString[type] << " " << pos << std::endl;
	}

	void Callback_Environnement::debugEvent(Environnement_Event::Type value, Square::EntityContain type, Coord pos, Coord newPos)
	{
		std::cout << _eventTypeString[value] << " " << _entityTypeString[type] << " " << pos << " " << newPos << std::endl;
	}

	void Callback_Environnement::debugEvent(Environnement_Event::Type value, Coord pos)
	{
		std::cout << _eventTypeString[value] << " " << pos << std::endl;
	}

	void Callback_Environnement::initEventTypeString() 
	{
		_eventTypeString[Environnement_Event::NONE] = "NONE";
		_eventTypeString[Environnement_Event::ENTITY_SPAWN] = "ENTITY_SPAWN";
		_eventTypeString[Environnement_Event::ENTITY_MOVE] = "ENTITY_MOVE";
		_eventTypeString[Environnement_Event::ENTITY_REPRODUCE] = "ENTITY_REPRODUCE";
		_eventTypeString[Environnement_Event::ENTITY_EAT] = "ENTITY_EAT";
		_eventTypeString[Environnement_Event::ENTITY_DEATH] = "ENTITY_DEATH";
		_eventTypeString[Environnement_Event::GRASS_UP] = "GRASS_UP";
		_eventTypeString[Environnement_Event::GRASS_DOWN] = "GRASS_DOWN";
	}

	void Callback_Environnement::initEntityTypeString() 
	{
		_entityTypeString[Square::SHEEP] = "SHEEP";
		_entityTypeString[Square::WOLF] = "WOLF";
	}
}
