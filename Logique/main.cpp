
#include <iostream>

#include "AlgoGen.hpp"
#include "Environnement.hpp"

int main(void) {
	AlgoGen gen;

	bool run = true;
	bool launched = false;
	std::string cin_buffer;
	boost::thread algo_thread;

	while (run)
	{
		std::cin >> cin_buffer;
		if (!launched && cin_buffer == "start")
		{
			launched = true;
			algo_thread = boost::thread(&AlgoGen::run, &gen);
			std::cout << "algogen start" << std::endl;
		}
		if (cin_buffer == "stop")
		{
			gen.stop();
			algo_thread.join();
			std::cout << "algogen stopped" << std::endl;
			launched = false;
		}
		if (cin_buffer == "exit")
		{
			run = false;
			std::cout << "exiting" << std::endl;
		}
			
	}	
	
	return 0;
}

