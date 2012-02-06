
#include <iostream>

#include "Environnement.hpp"
#include "Callback_Environnement.hpp"

int main(void) {
	Logique::Environnement env;
	env.setMetricSizeLimit(10);
	env.setEventSizeLimit(10);

	env.run();

	std::cin.get();
	return 0;
}

