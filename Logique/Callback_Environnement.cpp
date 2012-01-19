
#include "Callback_Environnement.hpp"

namespace Logique {

	Callback_Environnement::Callback_Environnement() 
	{}

	Callback_Environnement::~Callback_Environnement() {}

	Callback_Environnement::Callback_Environnement(const Callback_Environnement& orig) {}

	void Callback_Environnement::addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos, Coord newPos)
	{
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, id, type, pos, newPos));
		_mut.unlock();
	}

	void Callback_Environnement::addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos)
	{
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, id, type, pos));
		_mut.unlock();
	}
	void Callback_Environnement::addAction(Environnement_Event::Type value, Coord pos)
	{
		_mut.lock();
		_eventQueue.push_back(Environnement_Event(value, pos));
		_mut.unlock();
	}

	DequeProxy< Environnement_Event >&& Callback_Environnement::getEventProxy() {
		return DequeProxy< Environnement_Event >(_eventQueue, _mut);
	}
}
