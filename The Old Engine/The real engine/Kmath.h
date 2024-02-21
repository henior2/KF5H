#pragma once
#include "mat.h"
#include "vec.h"

class Kmath
{
public:
	static double Radians(const double& degries);
	static mat LookAt(const vec& Eye, const vec& Center, const vec& Up);
	static mat Perspective(const float& Fov, const float& AspectRatio, const float& NearPlane, const float& FarPlane);
	static mat Ortho(const float& Left, const float& Right, const float& Bottom, const float& Top, const float& Near, const float& Far);
};

