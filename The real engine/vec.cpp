#include "vec.h"

vec::vec()
	:size(0), array(new float[size]), x(*array), y(*(array + 1)), z(*(array + 2)), w(*(array + 3))
{}

vec::vec(const unsigned int& Size) 
	:size(Size), array(new float[size]), x(*array), y(*(array + 1)), z(*(array + 2)), w(*(array + 3))
{}

vec::vec(const float& value, const unsigned int& Size)
	:size(Size), array(new float[size]), x(*array), y(*(array + 1)), z(*(array + 2)), w(*(array + 3))
{

	for (int i = 0; i < size; i++) {
		array[i] = value;
	}

//	GenerateValues();
}

vec::vec(const float& valueX, const float& valueY, const float& valueZ)
	:size(3), array(new float[size]), x(*array), y(*(array + 1)), z(*(array + 2)), w(*(array + 3))
{

	array[0] = valueX;

	array[1] = valueY;

	array[2] = valueZ;

//	GenerateValues();
}

void vec::operator=(const vec& second) {
	if (&second != this) {
		// If sizes are different, adjust the size of the current object
		if (second.size != this->size) {
			
			//delete[] array;
			size = second.size;
			array = new float[size];
			GenerateValues();
		}

		for (int i = 0; i < size; i++) {
			array[i] = second.array[i];
		}
	}
}

vec vec::operator-() const {
	vec result(0, size);
	result = *this;

	for (int i = 0; i < size; i++)
	{
		result.array[i] = -result.array[i];
	}

	return result;
}

vec vec::operator+(const float& second) const {
	vec result(0, size);
	result = *this;
	for (int i = 0; i < size; i++)
	{
		array[i] += second;
	}
	return result;
}

vec vec::operator-(const float& second)const {
	return *this + (-second);
}

vec vec::operator+(const vec& second) const {
	if (size != second.size) {
		return *this;
	}
	vec result(0, size);
	result = *this;

	for (int i = 0; i < size; i++) {
		result.array[i] += second.array[i];
	}

	return result;
}

vec vec::operator-(const vec& second) const {
	return *this + (-second);
}

float vec::Length() const {
	float result = 0.0f;
	for (int i = 0; i < size; i++) {
		result += array[i] * array[i];
	}
	return sqrt(result);
}

vec vec::operator*(const float& second) const {
	vec result(0, size);
	result = *this;

	for (int i = 0; i < size; i++) {
		result.array[i] *= second;
	}

	return result;
}

vec vec::operator*(const mat& second) const {
	vec result(4);
	result = second * *this;
}

void vec::operator+=(const vec& second) {
	*this = *this + second;
}

void vec::operator-=(const vec& second) {
	*this = *this - second;
}

vec vec::Normalize() const {
	vec result(0, size);
	result = *this;

	float length = result.Length();

	if (length == 0) {
		return vec(0, size);
	}

	for (int i = 0; i < size; i++) {
		result.array[i] /= length;
	}

	return result;
}

vec vec::addNeutralizer() const {
	vec Neutralized = vec(0, size + 1);
	for (int i = 0; i < size; i++) {
		Neutralized.array[i] = this->array[i];
	}
	Neutralized.array[size] = 1;

	return Neutralized;
}


void vec::GenerateValues() {
	if (size >= 1) {
		x = *array;
	}

	if (size >= 2) {
		y = *(array + 1);
	}

	if (size >= 3) {
		z = *(array + 2);
	}

	if (size >= 4) {
		w = *(array + 3);
	}
}

vec vec::Cross(const vec& A, const vec& B) {
	vec result = vec(0, 3);
	if (A.size != 3 || B.size != 3) {
		return result;
	}

	result.x = A.y * B.z - A.z * B.y;
	result.y = A.z * B.x - A.x * B.z;
	result.z = A.x * B.y - A.y * B.x;

	return result;
}

float vec::Dot(const vec& A, const vec& B) {
	const float L1 = A.Length();
	const float L2 = B.Length();

	float result;
	result = A.x * B.x + A.y + B.y + A.z + B.z;
	result *= L1 * L2;

	return result;
}