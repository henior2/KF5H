#include "Camera.h"

Camera::Camera(const vec& position3, const vec& up, const float& yaw, const float& pitch)
	: Front(vec(0, 0, -1)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), perspective(true), cameraWidth(800), cameraHeight(600), Position(position3), WorldUp(up), Yaw(yaw), Pitch(pitch)
{
	UpdateCameraVectors();
}

mat Camera::GetViewMatrix() const {
	return Kmath::LookAt(Position, Position + Front, Up);
}

mat Camera::GetProjectionMatrix() const {
	if (perspective)
		return Kmath::Perspective(Kmath::Radians(90.0f), 16.0f / 9.0f, 0.1f, 100.0f);
		//return Kmath::Perspective(0.01f, 100.0f, 1.6f, -1.6f, 0.9f, -0.9f);
	else
		Kmath::Ortho(-cameraWidth, cameraWidth, -cameraHeight, cameraHeight, 0.1f, 100.0f);
}

void Camera::MoveCamera(const Camera_Movement& direction, const float& dt) {
	const float velocity = MovementSpeed * dt;
	switch (direction)
	{
	case(FORWARD):
		Position += Front * velocity;
		break;
	case(BACKWARD):
		Position -= Front * velocity;
		break;
	case(LEFT):
		Position -= Right * velocity;
		break;
	case(RIGHT):
		Position += Right * velocity;
		break;
	default:
		break;
	}
}

void Camera::RotateCamera(float xoffset, float yoffset, const bool& ConstrainPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (ConstrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ZoomCamera(const float& yoffset) {
	Zoom -= yoffset;

	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::UpdateCameraVectors() {
	vec front(3);

	front.x = cos(Kmath::Radians(Yaw)) * cos(Kmath::Radians(Pitch));
	front.y = sin(Kmath::Radians(Pitch));
	front.z = sin(Kmath::Radians(Yaw)) * cos(Kmath::Radians(Pitch));
	Front = front.Normalize();

	Right = vec::Cross(Front, WorldUp).Normalize();
	Up = vec::Cross(Right, Front).Normalize();
}