
#include <iostream>

#include "AlgoGen.hpp"
#include "Environnement.hpp"

int main(void) {
	AlgoGen gen;

	//Logique::Environnement* gen = 0;

	bool run = true;
	bool launched = false;
	std::string cin_buffer;
	boost::thread algo_thread;

	while (run)
	{
		std::cin >> cin_buffer;
		if (!launched && cin_buffer == "start")
		{
			std::cout << "algogen start" << std::endl;
			//gen = new Logique::Environnement();
			launched = true;
			algo_thread = boost::thread(&AlgoGen::run, &gen);			
			//algo_thread = boost::thread(&Logique::Environnement::run, gen);			
		}
		if (cin_buffer == "stop")
		{
			std::cout << "algogen stopped" << std::endl;
			gen.stop();
			//gen->stop();
			algo_thread.join();			
			//delete gen;
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

