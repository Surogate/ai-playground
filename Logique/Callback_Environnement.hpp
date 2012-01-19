
#ifndef CALLBACK_ENVIRONNEMENT
#define CALLBACK_ENVIRONNEMENT

#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <deque>

#include "Entity.hpp"
#include "Board.hpp"
#include "Singleton.hpp"
#include "Environnement_Event.h"
#include "EventProxy.hpp"

namespace Logique {

	class Callback_Environnement : public Singleton< Callback_Environnement > {
		SINGLETON_CLASS(Callback_Environnement);
	public:

		typedef std::deque< Environnement_Event > EventDeque;

		void addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos, Coord newPos);
		void addAction(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos);
		void addAction(Environnement_Event::Type value, Coord pos);

		DequeProxy< Environnement_Event >&& getEventProxy();

		void debugEvent(const Environnement_Event& ev);

	private:
		EventDeque _eventQueue;
		boost::mutex _mut;
		boost::array< const char *, Environnement_Event::TYPE_SIZE > _eventTypeString;
		boost::array< const char *, Square::ENTITY_CONTAINER_SIZE > _entityTypeString;

		Callback_Environnement();
		~Callback_Environnement();
		Callback_Environnement(const Callback_Environnement& value);

		void initEventTypeString();
		void initEntityTypeString();

		void debugEvent(Environnement_Event::Type, Square::EntityContain type, Coord pos);
		void debugEvent(Environnement_Event::Type, Square::EntityContain type, Coord pos, Coord newPos);
		void debugEvent(Environnement_Event::Type, Coord pos);
	};

}

#endif /* ! CALLBACK_ENVIRONNEMENT */