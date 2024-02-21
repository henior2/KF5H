#pragma once
#include "vec.h"
#include "mat.h"
#include "Kmath.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 25.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    // atrybuty kamery
    vec Position = vec(3);
    vec Front = vec(3);
    vec Up = vec(3);
    vec Right = vec(3);
    vec WorldUp = vec(3);

    // euler Angles
    float Yaw;
    float Pitch;
    // opcje kamery
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool perspective;

    float cameraWidth;
    float cameraHeight;

    Camera(const vec& position3 = vec(0, 3), const vec& up = vec(0, 1, 0), const float& yaw = YAW, const float& pitch = PITCH);

    mat GetViewMatrix() const;
    mat GetProjectionMatrix() const;

    void MoveCamera(const Camera_Movement& direction, const float& dt);

    void RotateCamera(float xoffset, float yoffset, const bool& ConstrainPitch = true);

    void ZoomCamera(const float& yoffset);

private:
    void UpdateCameraVectors();
};

