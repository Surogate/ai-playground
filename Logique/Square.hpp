
#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <boost/array.hpp>

#include <iostream>

namespace Logique {

	class Entity;

	struct Square {

		enum EntityContain {
			SHEEP,
			WOLF,
			ENTITY_CONTAINER_SIZE
		};

		enum Constant {
			ODOUR_START = 0,
			ODOUR_MAX = 10,
			GRASS_START = 8,
			GRASS_INC = 1,
			GRASS_DEC = 2,
			GRASS_MIN = 5,
			GRASS_MAX = 15
		};

		enum {
			USEABLE_MASK = 1,
			GRASS_MASK = 2,
			SHEEP_MASK = 4,
			WOLF_MASK = 8
		};


		Square() : _grass_value(GRASS_START), _entityIn(), _odour(ODOUR_START)
		{
			for (unsigned int i = 0; i < ENTITY_CONTAINER_SIZE; ++i) {
				_entityIn[i] = 0;
			}
		}

		inline bool hasEntity() const
		{
			unsigned int i = 0;
			while (i < ENTITY_CONTAINER_SIZE && _entityIn[i] != 0)
				++i;

			return i == ENTITY_CONTAINER_SIZE;
		}

		inline bool hasEntity(const EntityContain& value) const {
			return _entityIn[value] != 0;
		}

		inline bool hasEntity(const EntityContain& value, Entity* set) {
			_entityIn[value] = set;
			return _entityIn[value] != 0;
		}

		inline const Entity* getEntity(const EntityContain& value) const {
			return _entityIn[value];
		}

		inline Entity* getEntity(const EntityContain& value) {
			return _entityIn[value];
		}

		inline ptrdiff_t getId(const EntityContain& value) const {
			return reinterpret_cast<ptrdiff_t>(_entityIn[value]);
		}

		inline bool hasGrass() const  {
			return _grass_value >= GRASS_MIN;
		}

		inline bool increaseGrass() {
			bool start = hasGrass();
			increaseGrass(GRASS_INC);
			return start != hasGrass();
		}

		inline bool decreaseGrass() {
			bool start = hasGrass();
			decreaseGrass(GRASS_DEC);
			return start != hasGrass();
		}

		inline bool hasGrass(bool set) {
			if (hasGrass() != set)
			{
				if (set) {
					increaseGrass(GRASS_MIN);
				} else {
					_grass_value = 0;
				}
			}
			return hasGrass();
		}

		inline bool hasSheep() const  {
			return hasEntity(SHEEP);
		}

		inline bool hasWolf() const {
			return hasEntity(WOLF);
		}

		inline operator int() const {
			return getInt();
		}

		inline int getInt() const {
			int value = _odour;
			value = value << 4;

			if (hasGrass())
				value |= GRASS_MASK;

			if (hasSheep())
				value |= SHEEP_MASK;

			if (hasWolf())
				value |= WOLF_MASK;

			return value;
		}

		static inline int getIntmax()
		{
			int value = ODOUR_MAX;
			value = value << 4;

			value |= USEABLE_MASK;
			value |= GRASS_MASK;
			value |= SHEEP_MASK;
			value |= WOLF_MASK;

			return value;
		}

		inline void addOdour(unsigned int odour) {
			_odour += odour;
			if (_odour > ODOUR_MAX) {
				_odour = ODOUR_MAX;
			}
		}

		inline void decreaseOdour(unsigned int odour) {
			if (odour < _odour)
				_odour -= odour;
			else
				_odour = 0;
		}

		inline const unsigned int& odour() const {
			return _odour;
		}

		inline unsigned int& odour() {
			return _odour;
		}

		inline void dumpOdour() const {
			std::cout << _odour;
		}

		inline void dumpSheep() const {
			if (hasSheep()) {
				std::cout << "S";
			} else {
				std::cout << "O";
			}
		}

		inline void dumpWolf() const {
			if (hasWolf()) {
				std::cout << "W";
			} else {
				std::cout << "O";
			}
		}

		inline void dumpGrass() const {
			if (hasGrass()) {
				std::cout << "G";
			} else {
				std::cout << "O";
			}
		}

	private:
		unsigned int _grass_value;
		boost::array<Entity*, ENTITY_CONTAINER_SIZE> _entityIn;
		unsigned int _odour;

		void increaseGrass(unsigned int value) {
			_grass_value += value;
			if (_grass_value > GRASS_MAX) {
				_grass_value = GRASS_MAX;
			}
		}

		void decreaseGrass(unsigned int value) {
			if (value <= _grass_value) {
				_grass_value -= value;
			} else {
				_grass_value = 0;
			}
		}
	};

}

#endif /* !SQUARE_HPP */

