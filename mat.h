#pragma once
#include "vec.h"
#include "Quaternion.h"
class mat
{
public:
	unsigned int size;

	float** array;
	mat(const float& num, const unsigned int& Size);

	void operator=(const mat& second);
	
	mat operator*(const mat& second) const;
	mat operator*(const float& second) const;
	mat operator*(const vec& second) const;

	mat operator+(const mat& second) const;
	mat operator-(const mat& second) const;
	mat Translate(vec& translateVec);
	mat Rotate(vec& rotateVec);
	mat Scale(vec& scaleVec);

};

