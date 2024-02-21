#pragma once

#include <cmath>
class vec
{
public:
	unsigned int size;

	float* array;

	float& x, y, z, w;

	vec(const float& value, const unsigned int& Size);

	void operator=(const vec& second);

	vec operator+(const float& second) const;
	vec operator-(const float& second) const;

	vec operator-() const;

	vec operator+(const vec& second) const;
	vec operator-(const vec& second) const;

	float Length() const;
	vec Normalize() const;
	vec Dot() const;
	vec Cross() const;
private:

	void GenerateValues();
};

