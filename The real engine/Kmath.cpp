#include "Kmath.h"


double Kmath::Radians(const double& degrees) {
	double deg = (M_PI / 180.0) * degrees;
	return deg;
}

mat Kmath::LookAt(const vec& Eye, const vec& Center, const vec& Up) {
	vec forward(3);
	vec right(3);
	vec newUp(3);
	forward = (Center - Eye).Normalize();
	right = vec::Cross(Up, forward).Normalize();
	newUp = vec::Cross(forward, right);

	mat ViewMatrix(1, 4);

	ViewMatrix.array[0][0] = right.x;
	ViewMatrix.array[1][0] = right.y;
	ViewMatrix.array[2][0] = right.z;

	ViewMatrix.array[0][1] = newUp.x;
	ViewMatrix.array[1][1] = newUp.y;
	ViewMatrix.array[2][1] = newUp.z;

	ViewMatrix.array[0][2] = -forward.x;
	ViewMatrix.array[1][2] = -forward.y;
	ViewMatrix.array[2][2] = -forward.z;

	ViewMatrix.array[3][0] = -vec::Dot(right, Eye);
	ViewMatrix.array[3][1] = -vec::Dot(newUp, Eye);
	ViewMatrix.array[3][2] = -vec::Dot(forward, Eye);

	return ViewMatrix;
}

mat Kmath::Perspective(const float& Fov, const float& AspectRatio, const float& NearPlane, const float& FarPlane) {
	const float f = 1.0f / tan(Fov / 2.0f);

	mat PerspectiveMatrix(4);

	PerspectiveMatrix.array[0][0] = f / AspectRatio;
	PerspectiveMatrix.array[1][1] = f;
	PerspectiveMatrix.array[2][2] = (FarPlane + NearPlane) / (NearPlane - FarPlane);
	PerspectiveMatrix.array[2][3] = (2 * FarPlane * NearPlane) / (NearPlane - FarPlane);
	PerspectiveMatrix.array[3][2] = -1.0f;

	return PerspectiveMatrix;
}

mat Kmath::Ortho(const float& Left, const float& Right, const float& Bottom, const float& Top, const float& Near, const float& Far) {
	mat OrthoMatrix(1, 4);

	OrthoMatrix.array[0][0] = 2 / (Right - Left);
	OrthoMatrix.array[1][1] = 2 / (Top - Bottom);
	OrthoMatrix.array[2][2] = -2 / (Far - Near);

	OrthoMatrix.array[0][3] = -(Right + Left) / (Right - Left);
	OrthoMatrix.array[1][3] = -(Top + Bottom) / (Top - Bottom);
	OrthoMatrix.array[2][3] = -(Far + Near) / (Far - Near);

	return OrthoMatrix;
}