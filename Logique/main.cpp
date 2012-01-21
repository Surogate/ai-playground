
#include <iostream>

#include "Environnement.hpp"
#include "Callback_Environnement.hpp"

int main(void) {
	Logique::Environnement env;

	Logique::Callback_Environnement::EventProxy proxy = Logique::Callback_Environnement::getInstance().getEventProxy();

	std::cin.get();
	return 0;
}

