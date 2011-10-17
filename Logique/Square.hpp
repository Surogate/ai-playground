
#ifndef SQUARE_HPP
#define SQUARE_HPP

namespace Logique {

	struct Square {
		enum { ODOUR_MAX = 10 };

		Square() : odour_(0), useable_(true), hasGrass_(false), hasSheep_(false), hasWolf_(false)
		{}

		Square(bool useable) : odour_(0), useable_(useable), hasGrass_(false), hasSheep_(false), hasWolf_(false)
		{}

		inline bool useable() const {
			return useable_;
		}

		inline bool hasGrass() const  {
			return hasGrass_;
		}

		inline bool hasGrass(bool set) {
			hasGrass_ = useable_ && set;
			return hasGrass_;
		}

		inline bool hasSheep() const  {
			return hasSheep_;
		}

		inline bool hasSheep(bool set) {
			hasSheep_ = useable_ && set; 
			return hasSheep_;
		}

		inline bool hasWolf() const {
			return hasWolf_;
		}

		inline bool hasWolf(bool set) {
			hasWolf_ = useable_ && set;
			return hasWolf_;
		}

		inline int getInt() const {
			int value = odour_;
			value = value << 4;
			
			if (useable_)
				value |= USEABLE;

			if (hasGrass_)
				value |= GRASS;

			if (hasSheep_)
				value |= SHEEP;

			if (hasWolf_)
				value |= WOLF;

			return value;
		}

		inline void addOdour(unsigned int odour) {
			odour_ += odour;
			if (odour_ > ODOUR_MAX) {
				odour_ = ODOUR_MAX;
			}
		}

		unsigned int odour_;

	private:
		enum {
			USEABLE = 1,
			GRASS = 2,
			SHEEP = 4,
			WOLF = 8
		};

		
		const bool useable_;
		bool hasGrass_;	
		bool hasSheep_;
		bool hasWolf_;
	};

}

#endif /* !SQUARE_HPP */

