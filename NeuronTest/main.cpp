
#include <iostream>

#include "NeuralNetwork.hpp"
#include "fann.h"
#include "fann_cpp.h"

int main(void)
{
	FANN::neural_net test;
	FANN::neural_net test2;
	NeuralNetwork net(3, 2, 3, 1);
	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 500000;
	const unsigned int epochs_between_reports = 1000;
	fann_train_on_file(net.getFann(), "xor.data", max_epochs,
		epochs_between_reports, desired_error);
	net.saveToFile("xor_float.net");
	std::cin.get();
	return 0;
}