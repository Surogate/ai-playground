
#include <iostream>

#include "Environnement.hpp"
#include "Callback_Environnement.hpp"

int main(void) {
	Logique::Environnement env;

	env.run();

	std::cin.get();
	return 0;
}

