#pragma once
#include "vec.h"

class mat
{
public:
	unsigned int size;

	float** array = nullptr;
	mat(const float& num, const unsigned int& Size);
	mat(const unsigned int& Size);

	~mat();

	void operator=(const mat& second);
	
	mat operator*(const mat& second) const;
	mat operator*(const float& second) const;
	vec operator*(const vec& second) const;

	mat operator+(const mat& second) const;
	mat operator-(const mat& second) const;
	mat Translate(const vec& translateVec);
	mat RotateX(const float& rotateValue);
	mat RotateY(const float& rotateValue);
	mat RotateZ(const float& rotateValue);
	void Rotate(const vec& rotateVec);
	mat Scale(const vec& scaleVec);
};

