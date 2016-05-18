#include "Vector3.h"
#include "Quaternion.h"

Vector3::Vector3() : glm::vec3(0.0f)
{
}

Vector3::Vector3(const glm::vec3 &v) : glm::vec3(v)
{
}

Vector3::Vector3(float a) : glm::vec3(a)
{

}

Vector3::Vector3(float x, float y, float z) : glm::vec3(x,y,z)
{

}

float Vector3::Length() const
{
    return glm::length(glm::vec3(x,y,z));
}

Vector3 Vector3::Normalized() const
{
    return Vector3(glm::normalize(glm::vec3(*this)));
}

Vector3 Vector3::ToDegrees() const
{
    return Vector3(glm::degrees(glm::vec3(*this)));
}

Vector3 Vector3::ToRadians() const
{
    return Vector3(glm::radians(glm::vec3(*this)));
}

Vector3 operator*(Quaternion q, const Vector3 &rhs)
{
    return Vector3(q * glm::vec3(rhs.x, rhs.y, rhs.z));
}


Vector3 operator+(const Vector3 & v1, const Vector3 &v2)
{
    return Vector3(glm::vec3(v1) + glm::vec3(v2));
}


Vector3 operator*(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(glm::vec3(v1) * glm::vec3(v2));
}


Vector3 operator-(const Vector3 &v)
{
    return Vector3(-glm::vec3(v));
}


Vector3 operator*(float a, const Vector3 &v)
{
    return Vector3(a * glm::vec3(v));
}


Vector3 operator*(const Vector3 &v, float a)
{
    return Vector3(a * glm::vec3(v));
}


Vector3 operator/(float a, const Vector3 &v)
{
    return Vector3(a / glm::vec3(v));
}


Vector3 operator/(const Vector3 &v, float a)
{
    return Vector3(glm::vec3(v) / a);
}


Vector3 operator/(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(glm::vec3(v1) * glm::vec3(v2));
}


Vector3 &operator+=(Vector3 &lhs, const Vector3 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}


Vector3 &operator-=(Vector3 &lhs, const Vector3 &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Vector3 &operator*=(Vector3 &lhs, const Vector3 &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}


Vector3 &operator/=(Vector3 &lhs, const Vector3 &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
}
