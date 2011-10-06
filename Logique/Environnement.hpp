
#ifndef ENVIRONNEMENT_HPP
#define ENVIRONNEMENT_HPP

#include <list>
#include <memory>
#include <array>

#include "Square.hpp"
#include "Action.hpp"
#include "Entity.hpp"

class Environnement {
public:
	void run();
	void setBaseTime(int time);
private:
	std::array<Square, 20> board_;

	std::list< std::shared_ptr<Entity> > entityList_;
	std::list< Action > actionList_;
};


#endif /* !ENVIRONNEMENT_HPP */

