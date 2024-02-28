#pragma once
#include <cmath>
class vec
{
public:
	unsigned int size;

	float* array = nullptr;

	float& x;
	float& y;
	float& z;
	float& w;

	vec();
	vec(const unsigned int& Size);
	vec(const float& value, const unsigned int& Size);
	vec(const float& valueX, const float& valueY, const float& valueZ);
	~vec();

	void operator=(const vec& second);

	vec operator+(const float& second) const;
	vec operator-(const float& second) const;

	vec operator-() const;

	vec operator+(const vec& second) const;
	vec operator-(const vec& second) const;

	vec operator*(const float& second) const;

	void operator +=(const vec& second);
	void operator -=(const vec& second);

	bool operator ==(const vec& second) const;

	//did bro just multiply vecotrs (FBI's coming for him) (he has no idea)
	vec operator&(const vec& second) const;

	float Length() const;
	vec Normalize() const;
	static float Dot(const vec& A, const vec& B);
	static vec Cross(const vec& first, const vec& second);
	vec addNeutralizer() const;
private:

	void GenerateValues();
};

