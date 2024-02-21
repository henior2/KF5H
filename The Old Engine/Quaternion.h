#pragma once
#ifndef M_PI
#define M_PI 3.14159265358979323846
#include <cmath>
class Quaternion
{
public:
    float w, x, y, z;

    Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {}

    // Normalize the quaternion
    void normalize() {
        float mag = std::sqrt(w * w + x * x + y * y + z * z);
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
    }

    // Convert quaternion to rotation matrix
    void toMatrix(float matrix[3][3]) const {
        matrix[0][0] = 1 - 2 * (y * y + z * z);
        matrix[0][1] = 2 * (x * y - w * z);
        matrix[0][2] = 2 * (x * z + w * y);

        matrix[1][0] = 2 * (x * y + w * z);
        matrix[1][1] = 1 - 2 * (x * x + z * z);
        matrix[1][2] = 2 * (y * z - w * x);

        matrix[2][0] = 2 * (x * z - w * y);
        matrix[2][1] = 2 * (y * z + w * x);
        matrix[2][2] = 1 - 2 * (x * x + y * y);
    }

    // Convert Euler angles (in degrees) to quaternion
    static Quaternion fromEulerAngles(float roll, float pitch, float yaw) {
        // Convert degrees to radians
        roll = roll * static_cast<float>(M_PI / 180.0);
        pitch = pitch * static_cast<float>(M_PI / 180.0);
        yaw = yaw * static_cast<float>(M_PI / 180.0);

        float cy = (float) std::cos(yaw * 0.5);
        float sy = (float) std::sin(yaw * 0.5);
        float cp = (float) std::cos(pitch * 0.5);
        float sp = (float) std::sin(pitch * 0.5);
        float cr = (float) std::cos(roll * 0.5);
        float sr = (float) std::sin(roll * 0.5);

        float qw = cr * cp * cy + sr * sp * sy;
        float qx = sr * cp * cy - cr * sp * sy;
        float qy = cr * sp * cy + sr * cp * sy;
        float qz = cr * cp * sy - sr * sp * cy;

        return Quaternion(qw, qx, qy, qz);
    }
};

#endif