#include "mat.h"

mat::mat(const float& num, const unsigned int& Size)
	: size(Size)
{
	array = new float* [Size];
	for (unsigned int i = 0; i < Size; i++) {
		array[i] = new float[Size];
	}

	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++) {
			if (i == j)
				array[i][j] = num;
			else
				array[i][j] = 0;
		}
	}
}

void mat::operator=(const mat& second) {
	if (&second == this || second.size != this->size)
		return;

	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			array[i][j] = second.array[i][j];
		}
	}
}

mat mat::operator+(const mat& second) const {
	if (this->size != second.size) {
		return *this;
	}
	mat result(0.0f, size);

	for (unsigned int i = 0; i < size; i++){
		for (unsigned int j = 0; j < size; j++) {
			result.array[i][j] = array[i][j] + second.array[i][j];
		}
	}

	return result;
}

mat mat::operator-(const mat& second) const {
	if (this->size != second.size) {
		return *this;
	}
	mat result(0.0f, size);

	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			result.array[i][j] = array[i][j] - second.array[i][j];
		}
	}

	return result;
}

mat mat::operator*(const mat& second) const{
	if (this->size != second.size) {
		return *this;
	}
	mat result(0.0f, size);

	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			for (unsigned int k = 0; k < size; k++) {
				result.array[i][j] += this->array[i][k] * second.array[k][j];
			}
		}
	}

	return result;
}

mat mat::operator*(const float& second) const {
	mat result(0.0f, size);

	result = *this;

	for (unsigned int i = 0; i < size; i++) {
		for (unsigned int j = 0; j < size; j++) {
			result.array[i][j] *= second;
		}
	}

	return result;
}

mat mat::operator*(const vec& second) const {
	if (this->size != second.size) {
		return *this;
	}

	mat result(0.0f, size);

	result = *this;

	for (unsigned int i = 0; i < size; i++) {
		result.array[i][i] *= second.array[i];
	}

	return result;
}

mat mat::Translate(vec& translateVec) {

	if (translateVec.size != size) {
		return *this;
	}

	mat result(1.0f, size);

	for (unsigned int i = 0; i < size; i++) {
		result.array[i][size - 1] += translateVec.array[i];
	}

	return result;
}
mat mat::Rotate(vec& rotateVec) {
	if (rotateVec.size != 3) {
		return *this;
	}

	mat result(1.0f, size);

	Quaternion quaternion = Quaternion::fromEulerAngles(rotateVec.z, rotateVec.x, rotateVec.y);
	quaternion.normalize();
	float matrix[3][3];
	quaternion.toMatrix(matrix);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.array[i][j] = matrix[i][j];
		}
	}

	return result;
}
mat mat::Scale(vec& scaleVec) {
	if (scaleVec.size != 3) {
		return * this;
	}

	mat result(1.0f, size);

	for (unsigned int i = 0; i < size; i++) {
		result.array[i][i] = scaleVec.array[i];
	}

	return result;
}