
#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <boost/array.hpp>

#include <iostream>

namespace Logique {

	class Entity;

	struct Square {
		enum { ODOUR_MAX = 10 };

		enum EntityContain {
			SHEEP,
			WOLF,
			ENTITY_CONTAINER_SIZE
		};

		enum {
			USEABLE_MASK = 1,
			GRASS_MASK = 2,
			SHEEP_MASK = 4,
			WOLF_MASK = 8
		};


		Square() : _useable(true), _hasGrass(true), _odour(0)
		{
			for (unsigned int i = 0; i < ENTITY_CONTAINER_SIZE; ++i) {
				_entityIn[i] = 0;
			}
		}

		Square(bool useable) : _useable(useable), _hasGrass(true), _odour(0)
		{
			for (unsigned int i = 0; i < ENTITY_CONTAINER_SIZE; ++i) {
				_entityIn[i] = 0;
			}
		}

		inline bool useable() const {
			return _useable;
		}

		inline bool hasEntity(const EntityContain& value) {
			return _entityIn[value] != 0;
		}

		inline bool hasEntity(const EntityContain& value, Entity* set) {
			if (_useable)
				_entityIn[value] = set;
			return _entityIn[value] != 0;
		}

		inline Entity* getEntity(const EntityContain& value) {
			return _entityIn[value];
		}

		inline bool hasGrass() const  {
			return _hasGrass;
		}

		inline bool hasGrass(bool set) {
			_hasGrass = _useable && set;
			return _hasGrass;
		}

		inline bool hasSheep() const  {
			return _entityIn[SHEEP] != 0;
		}

		inline bool hasSheep(Entity* set) {
			if (_useable)
				_entityIn[SHEEP] = set; 
			return _entityIn[SHEEP] != 0;
		}

		inline bool hasWolf() const {
			return _entityIn[WOLF] != 0;
		}

		inline bool hasWolf(Entity* set) {
			if (_useable)
				_entityIn[WOLF] = set;
			return _entityIn[WOLF] != 0;
		}

		inline operator int() const {
			return getInt();
		}

		inline int getInt() const {
			int value = _odour;
			value = value << 4;
			
			if (_useable)
				value |= USEABLE_MASK;

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

		inline void dump() const {
			std::cout << _odour;
		}

		inline void dumpSheep() const {
			if (hasSheep()) {
				std::cout << "M";
			} else {
				std::cout << "O";
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

	private:
		const bool _useable;
		bool _hasGrass;
		boost::array<Entity*, ENTITY_CONTAINER_SIZE> _entityIn;
		unsigned int _odour;
	};

}

#endif /* !SQUARE_HPP */

