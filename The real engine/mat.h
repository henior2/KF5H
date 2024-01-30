#pragma once
#include "vec.h"
#include "Quaternion.h"

class mat
{
public:
	unsigned int size;

	float** array;
	mat(const float& num, const unsigned int& Size);
	mat(const unsigned int& Size);

	void operator=(const mat& second);
	
	mat operator*(const mat& second) const;
	mat operator*(const float& second) const;
	vec operator*(const vec& second) const;

	mat operator+(const mat& second) const;
	mat operator-(const mat& second) const;
	mat Translate(const vec& translateVec);
	mat Rotate(const vec& rotateVec);
	mat Scale(const vec& scaleVec);
};

