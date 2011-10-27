
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <array>

template <typename Type, unsigned int ROWS, unsigned int COLUMS>
class Matrix : public std::array< std::array< Type, COLUMS >, ROWS >{
public:
	Matrix& operator+(const Matrix& value) {
		for (unsigned int ro = 0; ro < ROWS; ++ro) 
		{
			for (unsigned int col = 0; col < COLUMS; ++col)
				this->at(ro)[col] += value[ro][col];
		}
		return *this;
	}

	Matrix& operator-(const Matrix& value) {
		for (unsigned int ro = 0; ro < ROWS; ++ro) 
		{
			for (unsigned int col = 0; col < COLUMS; ++col)
				this->at(ro)[col] -= value[ro][col];
		}
		return *this;
	}

	Matrix& operator*(const Type& value) {
		for (unsigned int ro = 0; ro < ROWS; ++ro) 
		{
			for (unsigned int col = 0; col < COLUMS; ++col)
				this->at(ro)[col] *= value;
		}
		return *this;
	}

	template <unsigned int COLUMS_BUDDY>
	Matrix< Type, ROWS, COLUMS_BUDDY > operator*(const Matrix<Type, COLUMS, COLUMS_BUDDY>& value) {
		Matrix< Type, ROWS, COLUMS_BUDDY > result;

		for (unsigned int col_buddy = 0; col_buddy < COLUMS_BUDDY; ++col_buddy)
		{
			for (unsigned int row = 0; row < ROWS; ++row) 
			{
				result[row][col_buddy] = Type();
				for (unsigned int col = 0; col < COLUMS; ++col)
				{
					result[row][col_buddy] += this->at(row)[col] * value[col][col_buddy];
				}
			}
		}

		return result;
	}

};

template <typename Type, unsigned int ROWS, unsigned int COLUMS>
std::ostream& operator<<(std::ostream& cout, const Matrix<Type, ROWS, COLUMS>& value) {
	for (unsigned int i = 0; i < ROWS; ++i) {
		for (unsigned int o = 0; o < COLUMS; ++o)
			cout << value[i][o] << ' ';
		cout << std::endl;
	}
	return cout;
} 

#endif /* !MATRIX_HPP */
