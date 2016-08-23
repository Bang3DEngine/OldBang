#include "Vector3.h"

#include "String.h"
#include "Vector2.h"
#include "Quaternion.h"

const Vector3 Vector3::up      = Vector3( 0, 1,  0);
const Vector3 Vector3::down    = Vector3( 0,-1,  0);
const Vector3 Vector3::right   = Vector3( 1, 0,  0);
const Vector3 Vector3::left    = Vector3(-1, 0,  0);
const Vector3 Vector3::forward = Vector3( 0, 0, -1);
const Vector3 Vector3::back    = Vector3( 0, 0,  1);
const Vector3 Vector3::zero    = Vector3( 0, 0,  0);
const Vector3 Vector3::one     = Vector3( 1, 1,  1);
//*/

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

Vector3::Vector3(const Vector2 &v, float z) : Vector3(v.x, v.y, z)
{
}

float Vector3::Length() const
{
    return float(glm::sqrt(x*x + y*y + z*z));
}

void Vector3::Normalize()
{
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
}

Vector3 Vector3::Normalized() const
{
    Vector3 v(*this);
    v.Normalize();
    return v;
}

Vector3 Vector3::ToDegrees() const
{
    return Vector3(glm::degrees(glm::vec3(*this)));
}

Vector3 Vector3::ToRadians() const
{
    return Vector3(glm::radians(glm::vec3(*this)));
}

glm::vec3 Vector3::ToGlmVec3() const
{
    return glm::vec3(x,y,z);
}

String Vector3::ToString() const
{
    return "(" + String::ToString(x) + ", " +
                 String::ToString(y) + ", " +
                 String::ToString(z) + ")";
}

float Vector3::Distance(const Vector3 &p) const
{
    return Vector3::Distance(*this, p);
}

void Vector3::OrthoNormalize(Vector3 &v1, Vector3 &v2)
{
    //Not sure if this is working...
    v1.Normalize();
    v2.Normalize();
    Vector3 aux = Vector3::Cross(v1, v2).Normalized();
    v2 = Vector3::Cross(aux, v1).Normalized();
}

Vector3 Vector3::Lerp(const Vector3 &v1,
                      const Vector3 &v2,
                      float progression)
{
    return Vector3(glm::mix(glm::vec3(v1), glm::vec3(v2), progression));
}

Vector3 Vector3::Abs() const
{
    return Vector3(glm::abs(x), glm::abs(y), glm::abs(z));
}

Vector3 Vector3::Abs(const Vector3 &v)
{
    return v.Abs();
}

Vector3 Vector3::Cross(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(glm::cross(glm::vec3(v1), glm::vec3(v2)));
}

float Vector3::Dot(const Vector3 &v1, const Vector3 &v2)
{
    return glm::dot(glm::vec3(v1), glm::vec3(v2));
}

float Vector3::Distance(const Vector3 &v1, const Vector3 &v2)
{
    return (v1 - v2).Length();
}

Vector2 Vector3::xy() const
{
    return Vector2(x, y);
}








Vector3 operator*(Quaternion q, const Vector3 &rhs)
{
    return Vector3(q * glm::vec3(rhs.x, rhs.y, rhs.z));
}


Vector3 operator+(const Vector3 & v1, const Vector3 &v2)
{
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}


Vector3 operator*(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}


Vector3 operator-(const Vector3 &v)
{
    return Vector3(-v.x, -v.y, -v.z);
}


Vector3 operator*(float a, const Vector3 &v)
{
    return Vector3(a * v.x, a * v.y, a * v.z);
}


Vector3 operator*(const Vector3 &v, float a)
{
    return a * v;
}


Vector3 operator/(float a, const Vector3 &v)
{
    return Vector3(a / v.x, a / v.y, a / v.z);
}


Vector3 operator/(const Vector3 &v, float a)
{
    return Vector3(v.x / a, v.y / a, v.z / a);
}


Vector3 operator/(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
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


Vector3 operator+(float a, const Vector3 &v)
{
    return Vector3(a + v.x, a + v.y, a + v.z);
}


Vector3 operator+(const Vector3 &v, float a)
{
    return a + v;
}


Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}


Vector3 operator-(const Vector3 &v, float a)
{
    return Vector3(v.x - a, v.y - a, v.z - a);
}


Vector3 operator-(float a, const Vector3 &v)
{
    return Vector3(a - v.x, a - v.y, a - v.z);
}


Vector3 &operator+=(Vector3 &lhs, float a)
{
    lhs.x += a;
    lhs.y += a;
    lhs.z += a;
    return lhs;
}


Vector3 &operator-=(Vector3 &lhs, float a)
{
    lhs.x -= a;
    lhs.y -= a;
    lhs.z -= a;
    return lhs;
}


Vector3 &operator*=(Vector3 &lhs, float a)
{
    lhs.x *= a;
    lhs.y *= a;
    lhs.z *= a;
    return lhs;
}


Vector3 &operator/=(Vector3 &lhs, float a)
{
    lhs.x /= a;
    lhs.y /= a;
    lhs.z /= a;
    return lhs;
}

