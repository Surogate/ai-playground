
#include "DecisionTree.hpp"

namespace Logique {
	DecisionTree::DecisionTree() 
		: _moyenne(0.f)
	{
		_randomArray[Entity::MOVE_UP] = Entity::MOVE_UP;
		_randomArray[Entity::MOVE_DOWN] = Entity::MOVE_DOWN;
		_randomArray[Entity::MOVE_LEFT] = Entity::MOVE_LEFT;
		_randomArray[Entity::MOVE_RIGHT] = Entity::MOVE_RIGHT;
		_randomArray[Entity::EAT] = Entity::EAT;
		_randomArray[Entity::REPRODUCE] = Entity::REPRODUCE;
	}

	Entity::EntityAction DecisionTree::computeAction(int present, int haut, int gauche, int bas, int droite) {
		Answer ans = _id3.decide(present, haut, gauche, bas, droite);
		if (ans) {
			return *ans;
		}
		return Entity::EntityAction();
	}

	std::random_device _randomD;
	std::mt19937 _gen;
	std::uniform_int_distribution<unsigned int> _distri;

}


