
#include <iostream>

#include "Matrix.hpp"

int main(void) {
	Matrix<int, 3, 3> vector;
	Matrix<int, 3, 1> vector2;

	vector[0][0] = 1;
	vector[1][0] = 0;
	vector[2][0] = 0;

	vector[0][1] = 0;
	vector[1][1] = 1;
	vector[2][1] = 0;

	vector[0][2] = 0;
	vector[1][2] = 0;
	vector[2][2] = 1;

	vector2[0][0] = 1;
	vector2[1][0] = 3;
	vector2[2][0] = 2;


	std::cout << "vector un " << std::endl;
	std::cout << vector << std::endl;

	std::cout << "vector deux " << std::endl;
	std::cout << vector2 << std::endl;

	std::cout << "resultat" << std::endl;
	std::cout << vector * vector2 << std::endl;

	std::cin.get();
	return 0;
}