#include "Bang/Vector4.h"

#include "Bang/Math.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Matrix4.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"

const Vector4 Vector4::Up      = Vector4( 0,  1,  0, 0);
const Vector4 Vector4::Down    = Vector4( 0, -1,  0, 0);
const Vector4 Vector4::Right   = Vector4( 1,  0,  0, 0);
const Vector4 Vector4::Left    = Vector4(-1,  0,  0, 0);
const Vector4 Vector4::Forward = Vector4( 0,  0, -1, 0);
const Vector4 Vector4::Back    = Vector4( 0,  0,  1, 0);
const Vector4 Vector4::Zero    = Vector4( 0,  0,  0, 0);
const Vector4 Vector4::One     = Vector4( 1,  1,  1, 1);

Vector4::Vector4() : Vector4(0.0f)
{
}

Vector4::Vector4(const glm::vec4 &v) : Vector4(v.x, v.y, v.z, v.w)
{
}

Vector4::Vector4(float a) : Vector4(a, a, a, a)
{
}

Vector4::Vector4(float x, float y, float z, float w) : glm::vec4(x,y,z,w)
{
}

Vector4::Vector4(const Color &c) : Vector4(c.r, c.g, c.b, c.a)
{
}

Vector4::Vector4(const Vector3 &v, float w) : Vector4(v.x, v.y, v.z, w)
{
}

Vector4::Vector4(const Vector2 &v, float z, float w) : Vector4(v.x, v.y, z, w)
{
}

float Vector4::Length() const
{
    return float(Math::Sqrt(x*x + y*y + z*z + w*w));
}

void Vector4::Normalize()
{
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
}

Vector4 Vector4::Normalized() const
{
    Vector4 v(*this);
    v.Normalize();
    return v;
}

Vector4 Vector4::ToDegrees() const
{
    return Vector4( Math::Rad2Deg(x), Math::Rad2Deg(y), Math::Rad2Deg(z), Math::Rad2Deg(w));
}

Vector4 Vector4::ToRadians() const
{
    return Vector4( Math::Deg2Rad(x), Math::Deg2Rad(y), Math::Deg2Rad(z), Math::Deg2Rad(w));
}

glm::vec4 Vector4::ToGlmVec4() const
{
    return glm::vec4(x,y,z,w);
}

String Vector4::ToString() const
{
    return "(" + String::ToString(x) + ", " +
                 String::ToString(y) + ", " +
                 String::ToString(z) + ", " +
                 String::ToString(w) + ")";
}

float Vector4::Distance(const Vector4 &p) const
{
    return Vector4::Distance(*this, p);
}

Vector4 Vector4::Lerp(const Vector4 &v1,
                      const Vector4 &v2,
                      float progression)
{
    return Vector4(glm::mix(glm::vec4(v1), glm::vec4(v2), progression));
}

Vector4 Vector4::Abs() const
{
    return Vector4(Math::Abs(x), Math::Abs(y), Math::Abs(z), Math::Abs(w));
}

float *Vector4::Values() const
{
    return (float*) &x;
}

Vector4 Vector4::Abs(const Vector4 &v)
{
    return v.Abs();
}

float Vector4::Dot(const Vector4 &v1, const Vector4 &v2)
{
    return glm::dot(glm::vec4(v1), glm::vec4(v2));
}

float Vector4::Distance(const Vector4 &v1, const Vector4 &v2)
{
    return (v1 - v2).Length();
}

Vector2 Vector4::xy() const
{
    return Vector2(x, y);
}

Vector3 Vector4::xyz() const
{
    return Vector3(x, y, z);
}










Vector4 operator*(Quaternion q, const Vector4 &rhs)
{
    return Vector4(q * glm::vec4(rhs.x, rhs.y, rhs.z, rhs.w));
}


Vector4 operator+(const Vector4 & v1, const Vector4 &v2)
{
    return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w * v2.w);
}


Vector4 operator*(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}


Vector4 operator-(const Vector4 &v)
{
    return Vector4(-v.x, -v.y, -v.z, -v.w);
}


Vector4 operator*(float a, const Vector4 &v)
{
    return Vector4(a * v.x, a * v.y, a * v.z, a * v.w);
}


Vector4 operator*(const Vector4 &v, float a)
{
    return a * v;
}


Vector4 operator/(float a, const Vector4 &v)
{
    return Vector4(a / v.x, a / v.y, a / v.z, a / v.w);
}


Vector4 operator/(const Vector4 &v, float a)
{
    return Vector4(v.x / a, v.y / a, v.z / a, v.w / a);
}


Vector4 operator/(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}


Vector4 &operator+=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}


Vector4 &operator-=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

Vector4 &operator*=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    lhs.w *= rhs.w;
    return lhs;
}


Vector4 &operator/=(Vector4 &lhs, const Vector4 &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    lhs.w /= rhs.w;
    return lhs;
}


Vector4 operator+(float a, const Vector4 &v)
{
    return Vector4(a + v.x, a + v.y, a + v.z, a + v.w);
}


Vector4 operator+(const Vector4 &v, float a)
{
    return a + v;
}


Vector4 operator-(const Vector4 &v1, const Vector4 &v2)
{
    return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}


Vector4 operator*(const Matrix4 &m, const Vector4 &v)
{
    return Vector4((m[0].x * v.x) + (m[1].x * v.y) + (m[2].x * v.z) + (m[3].x * v.w),
                   (m[0].y * v.x) + (m[1].y * v.y) + (m[2].y * v.z) + (m[3].y * v.w),
                   (m[0].z * v.x) + (m[1].z * v.y) + (m[2].z * v.z) + (m[3].z * v.w),
                   (m[0].w * v.x) + (m[1].w * v.y) + (m[2].w * v.z) + (m[3].w * v.w));
}


Vector4 operator-(float a, const Vector4 &v)
{
    return Vector4(a - v.x, a - v.y, a - v.z, a - v.w);
}


Vector4 operator-(const Vector4 &v, float a)
{
    return Vector4(v.x - a, v.y - a, v.z - a, v.w - a);
}

Vector4 &operator+=(Vector4 &lhs, float a)
{
    lhs.x += a;
    lhs.y += a;
    lhs.z += a;
    lhs.w += a;
    return lhs;
}


Vector4 &operator-=(Vector4 &lhs, float a)
{
    lhs.x -= a;
    lhs.y -= a;
    lhs.z -= a;
    lhs.w -= a;
    return lhs;
}


Vector4 &operator*=(Vector4 &lhs, float a)
{
    lhs.x *= a;
    lhs.y *= a;
    lhs.z *= a;
    lhs.w *= a;
    return lhs;
}


Vector4 &operator/=(Vector4 &lhs, float a)
{
    lhs.x /= a;
    lhs.y /= a;
    lhs.z /= a;
    lhs.w /= a;
    return lhs;
}
