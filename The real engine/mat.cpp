#include "mat.h"

mat::mat(const float& num, const unsigned int& Size)
	: size(Size)
{
	array = new float* [Size];
	for (int i = 0; i < Size; i++)
		array[i] = new float[Size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			array[i][j] = (i == j) ? num : 0;
}

mat::mat(const unsigned int& Size) 
	: size(Size)
{
	array = new float* [Size];
	for (int i = 0; i < Size; i++)
		array[i] = new float[Size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			array[i][j] = 0;
}

mat::~mat() {
	for (int i = 0; i < size; i++) {
		try {
//			delete[] array[i];
		}
		catch(...){
		}
	}
	try {
//		delete[] array;
	}
	catch (...) {

	}
}

void mat::operator=(const mat& second) {
	if (&second == this || second.size != this->size)
		return;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			array[i][j] = second.array[i][j];
}

mat mat::operator+(const mat& second) const {
	if (this->size != second.size)
		return *this;

	mat result(0.0f, size);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result.array[i][j] = array[i][j] + second.array[i][j];

	return result;
}

mat mat::operator-(const mat& second) const {
	if (this->size != second.size)
		return *this;

	mat result(0.0f, size);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result.array[i][j] = array[i][j] - second.array[i][j];

	return result;
}

mat mat::operator*(const mat& second) const {
	if (this->size != second.size)
		return *this;

	mat result(0.0f, size);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
				result.array[i][j] += this->array[i][k] * second.array[k][j];

	return result;
}

mat mat::operator*(const float& second) const {
	mat result(0.0f, size);

	result = *this;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result.array[i][j] *= second;

	return result;
}

vec mat::operator*(const vec& second) const {
	vec New(0, size);
	if (this->size != second.size)
		New = second.addNeutralizer();
	else if (size == second.size)
		New = second;

	if (this->size != New.size)
		return New;

	vec result(0, size);

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			result.array[i] += this->array[i][j] * New.array[j];

	return result;
}

mat mat::Translate(const vec& translateVec) {
	if (translateVec.size != size - 1)
		return *this;

	mat result(1.0f, size);

	for (int i = 0; i < size - 1; i++)
		result.array[i][size - 1] += translateVec.array[i];

	return result;
}

mat mat::RotateX(const float& rotateValue) {
	mat result(1.0f, size);

	result.array[1][1] = cos(rotateValue);
	result.array[1][2] = -sin(rotateValue);
	result.array[2][1] = sin(rotateValue);
	result.array[2][2] = cos(rotateValue);

	return result;
}

mat mat::RotateY(const float& rotateValue) {
	mat result(1.0f, size);

	result.array[0][0] = cos(rotateValue);
	result.array[0][2] = sin(rotateValue);
	result.array[2][0] = -sin(rotateValue);
	result.array[2][2] = cos(rotateValue);

	return result;
}

mat mat::RotateZ(const float& rotateValue) {
	mat result(1.0f, size);

	result.array[0][0] = cos(rotateValue);
	result.array[0][1] = -sin(rotateValue);
	result.array[1][0] = sin(rotateValue);
	result.array[1][1] = cos(rotateValue);

	return result;
}

void mat::Rotate(const vec& rotateVec) {

	if (rotateVec.size != 3)
		return;

	*this = *this * RotateX(rotateVec.x);
	*this = *this * RotateY(rotateVec.y);
	*this = *this * RotateZ(rotateVec.z);
}
mat mat::Scale(const vec& scaleVec) {
	if (scaleVec.size != size-1)
		return *this;

	mat result(1.0f, size);

	for (int i = 0; i < size - 1; i++)
		result.array[i][i] = scaleVec.array[i];

	return result;
}