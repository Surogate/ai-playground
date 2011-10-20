
#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <array>

namespace Logique {

	struct Square {
		enum { ODOUR_MAX = 10 };

		enum EntityContain {
			SHEEP,
			WOLF,
			ENTITY_CONTAINER_SIZE
		};


		Square() : _odour(0), _useable(true), _hasGrass(false)
		{
			for (unsigned int i = 0; i < ENTITY_CONTAINER_SIZE; ++i) {
				_entityIn[i] = false;
			}
		}

		Square(bool useable) : _odour(0), _useable(useable), _hasGrass(false)
		{
			for (unsigned int i = 0; i < ENTITY_CONTAINER_SIZE; ++i) {
				_entityIn[i] = false;
			}
		}

		inline bool useable() const {
			return _useable;
		}

		inline bool hasEntity(const EntityContain& value) {
			return _entityIn[value];
		}

		inline bool hasEntity(const EntityContain& value, bool set) {
			_entityIn[value] = _useable && set;
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
			return _entityIn[SHEEP];
		}

		inline bool hasSheep(bool set) {
			_entityIn[SHEEP] = _useable && set; 
			return _entityIn[SHEEP];
		}

		inline bool hasWolf() const {
			return _entityIn[WOLF];
		}

		inline bool hasWolf(bool set) {
			_entityIn[WOLF] = _useable && set;
			return _entityIn[WOLF];
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

		inline void addOdour(unsigned int odour) {
			_odour += odour;
			if (_odour > ODOUR_MAX) {
				_odour = ODOUR_MAX;
			}
		}

		inline void dump() const {
			std::cout << _odour;
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
		enum {
			USEABLE_MASK = 1,
			GRASS_MASK = 2,
			SHEEP_MASK = 4,
			WOLF_MASK = 8
		};

		const bool _useable;
		bool _hasGrass;
		std::array<bool, ENTITY_CONTAINER_SIZE> _entityIn;
		unsigned int _odour;
	};

}

#endif /* !SQUARE_HPP */

