
#include <iostream>

#include "Environnement.hpp"

int main(void) {
	std::cout << "hello world" << std::endl;

	Environnement env;

	env.run();

	std::cin.get();
	return 0;
}

