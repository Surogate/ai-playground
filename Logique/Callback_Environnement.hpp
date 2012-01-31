
#ifndef CALLBACK_ENVIRONNEMENT
#define CALLBACK_ENVIRONNEMENT

#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/pool/pool_alloc.hpp>
#include <deque>

#include "Entity.hpp"
#include "Board.hpp"
#include "Singleton.hpp"
#include "Environnement_Event.h"
#include "EventProxy.hpp"
#include "Metric.hpp"

class Environnement;

namespace Logique {

	class Callback_Environnement {
	public:
		typedef std::deque< Environnement_Event, boost::pool_allocator<Environnement_Event> > EventDeque;
		typedef std::deque< Metric, boost::pool_allocator<Metric> > MetricDeque;
		typedef DequeProxy< EventDeque > EventProxy;
		typedef DequeProxy< MetricDeque > MetricProxy;

		Callback_Environnement();
		~Callback_Environnement();
		Callback_Environnement(const Callback_Environnement& value);

		void addEvent(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos, Coord newPos);
		void addEvent(Environnement_Event::Type value, Entity& id, Square::EntityContain type, Coord pos);
		void addEvent(Environnement_Event::Type value, Coord pos);
		void addMetric(const Metric& value);

		EventProxy getEventProxy();
		MetricProxy getMetricProxy();

		std::size_t getEventSize();
		std::size_t getMetricSize();

		void setEventSizeLimit(std::size_t limit);
		void setMetricSizeLimit(std::size_t limit);

		Environnement_Event popEventFromFront();
		Metric popMetricFromFront();

		void debugEvent(const Environnement_Event& ev);
		void debugMetric(const Metric& ev);
	private:
		EventDeque _eventQueue;
		MetricDeque _metricQueue;
		boost::mutex _mut;
		boost::mutex _metricMut;
		std::size_t _eventLimit;
		std::size_t _metricLimit;
		
		boost::array< const char *, Environnement_Event::TYPE_SIZE > _eventTypeString;
		boost::array< const char *, Square::ENTITY_CONTAINER_SIZE > _entityTypeString;

		void initEventTypeString();
		void initEntityTypeString();

		void debugEvent(Environnement_Event::Type, Square::EntityContain type, Coord pos);
		void debugEvent(Environnement_Event::Type, Square::EntityContain type, Coord pos, Coord newPos);
		void debugEvent(Environnement_Event::Type, Coord pos);
	};

}

#endif /* ! CALLBACK_ENVIRONNEMENT */