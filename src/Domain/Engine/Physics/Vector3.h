#ifndef VECTOR3_H
#define VECTOR3_H

#include "glm/glm.hpp"

class Quaternion;
class Vector3 : public glm::vec3
{
public:
    Vector3();
    explicit Vector3(const glm::vec3 &v);
    Vector3(float a);
    Vector3(float x, float y, float z);

    float Length() const;
    Vector3 Normalized() const;
    Vector3 ToDegrees() const;
    Vector3 ToRadians() const;
};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v);

Vector3 operator*(Quaternion q, const Vector3& rhs);
Vector3 operator*(float a, const Vector3& v);
Vector3 operator*(const Vector3& v, float a);
Vector3 operator*(const Vector3& v1, const Vector3& v2);

Vector3 operator/(float a, const Vector3& v);
Vector3 operator/(const Vector3& v, float a);
Vector3 operator/(const Vector3& v1, const Vector3& v2);

Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
Vector3& operator*=(Vector3& lhs, const Vector3& rhs);
Vector3& operator/=(Vector3& lhs, const Vector3& rhs);

#endif // VECTOR3_H
