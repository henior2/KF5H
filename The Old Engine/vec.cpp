#include "vec.h"

vec::vec(const float& value, const unsigned int& Size)
	:size(Size), x(array[0]), y(array[0]), z(array[0]), w(array[0])
{
	array = new float[size];

	for (int i = 0; i < size; i++) {
		array[i] = value;
	}

	GenerateValues();
}

void vec::operator=(const vec& second) {
	if (&second == this || second.size != this->size) {
		return;
	}

	for (int i = 0; i < size; i++)
	{
		array[i] = second.array[i];
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
}

vec vec::operator-(const vec& second) const {
	return *this + (-second);
}

float vec::Length() const {
	float result;

	float dist = x * x;
	switch (size)
	{
	case 2:
		dist += y * y;
	case 3:
		dist += z * z;
	case 4:
		dist += w * w;
	default:
		break;
	}

	result = sqrt(dist);
	return result;
}

vec vec::Normalize() const {
	vec result(0, size);
	result = *this;

	float length = result.Length();

	for (int i = 0; i < size; i++) {
		result.array[i] /= length;
	}

	return result;
}


void vec::GenerateValues() {
	switch (size)
	{
	case 2:
		y = array[1];
	case 3:
		z = array[2];
	case 4:
		w = array[3];
	default:
		break;
	}
}