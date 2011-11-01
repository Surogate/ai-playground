
#include "DecisionTree.hpp"

namespace Logique {
	DecisionTree::DecisionTree() 
		: _moyenne(0.f)
		, _randomD(), _gen(_randomD), _distri(0, Entity::ACTION_CONTAINER_SIZE - 1)
	{
	}

	Entity::EntityAction DecisionTree::randomAction() {
		return static_cast<Entity::EntityAction>(_distri(_gen));
	}

	Entity::EntityAction DecisionTree::computeAction(int present, int haut, int gauche, int bas, int droite) {
		Answer ans = _id3.decide(present, haut, gauche, bas, droite);
		if (ans) {
			return *ans;
		}
		return randomAction();
	}

	const float& DecisionTree::getMoy() const {
		return _moyenne;
	}

	void DecisionTree::addAction(int present, int haut, int gauche, int bas, int droite, Entity::EntityAction result) {
		_id3.addData(present, haut, gauche, bas, droite, result);
	}

	void DecisionTree::sendMoy(float value) {
		_moyenne = (_moyenne + value) / 2.f;
	}

	std::size_t DecisionTree::getSize() const {
		return _id3.size();
	}

}


