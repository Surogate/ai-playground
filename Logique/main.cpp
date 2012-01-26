
#include <iostream>

#include "Environnement.hpp"
#include "Callback_Environnement.hpp"

int main(void) {
	Logique::Environnement env;
	Logique::Callback_Environnement::getInstance().setMetricSizeLimit(10);
	Logique::Callback_Environnement::getInstance().setEventSizeLimit(10);

	env.run();

	std::cin.get();
	return 0;
}

