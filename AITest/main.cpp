
#include <iostream>
#include <list>

#include "Array.hpp"
#include "Gain.hpp"

enum Wind {
	Weak,
	Strong
};

enum PlayBall {
	No,
	Yes
};

void initWind(Array<Wind, 14>& array) {
	array[0] = Weak;
	array[1] = Strong;
	array[2] = Weak;
	array[3] = Weak;
	array[4] = Weak;
	array[5] = Strong;
	array[6] = Strong;
	array[7] = Weak;
	array[8] = Weak;
	array[9] = Weak;
	array[10] = Strong;
	array[11] = Strong;
	array[12] = Weak;
	array[13] = Strong;
}

void initResponse(Array<PlayBall, 14>& array) {
	array[0] = No;
	array[1] = No;
	array[2] = Yes;
	array[3] = Yes;
	array[4] = Yes;
	array[5] = No;
	array[6] = Yes;
	array[7] = No;
	array[8] = Yes;
	array[9] = Yes;
	array[10] = Yes;
	array[11] = Yes;
	array[12] = Yes;
	array[13] = No;
}

int main(void) {
	std::cout << "hello world" << std::endl;

	Array<Wind, 14> wind;
	Array<PlayBall, 14> response;

	initWind(wind);
	initResponse(response);

	Gain<Wind, PlayBall> calc(wind, response);

	float entropy_wind = calc.getResult();
	float entropy_global = calc.getGlobalEntropy();

	std::cout << "wind entropy " << entropy_wind << " entropy global " << entropy_global << std::endl;

	std::cin.get();
	return 0;
}