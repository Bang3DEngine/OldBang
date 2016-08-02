#include "Vector2.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"

const Vector2 Vector2::up      = Vector2( 0, 1);
const Vector2 Vector2::down    = Vector2( 0,-1);
const Vector2 Vector2::right   = Vector2( 1, 0);
const Vector2 Vector2::left    = Vector2(-1, 0);
const Vector2 Vector2::zero    = Vector2( 0, 0);
const Vector2 Vector2::one     = Vector2( 1, 1);

Vector2::Vector2() : glm::vec2(0.0f)
{
}

Vector2::Vector2(const glm::vec2 &v) : glm::vec2(v)
{
}

Vector2::Vector2(float a) : glm::vec2(a)
{
}

Vector2::Vector2(float x, float y) : glm::vec2(x,y)
{
}

float Vector2::Length() const
{
    return float(glm::sqrt(x*x + y*y));
}

void Vector2::Normalize()
{
    float l = Length();
    x /= l;
    y /= l;
}

Vector2 Vector2::Normalized() const
{
    Vector2 v(*this);
    v.Normalize();
    return v;
}

Vector2 Vector2::ToDegrees() const
{
    return Vector2(glm::degrees(glm::vec2(*this)));
}

Vector2 Vector2::ToRadians() const
{
    return Vector2(glm::radians(glm::vec2(*this)));
}

glm::vec2 Vector2::ToGlmVec2() const
{
    return glm::vec2(x,y);
}

float Vector2::Distance(const Vector2 &p) const
{
    return Vector2::Distance(*this, p);
}

Vector2 Vector2::Lerp(const Vector2 &v1,
                      const Vector2 &v2,
                      float progression)
{
    return Vector2(glm::mix(glm::vec2(v1), glm::vec2(v2), progression));
}

Vector2 Vector2::Abs() const
{
    return Vector2(glm::abs(x), glm::abs(y));
}

Vector2 Vector2::Abs(const Vector2 &v)
{
    return v.Abs();
}

float Vector2::Dot(const Vector2 &v1, const Vector2 &v2)
{
    return glm::dot(glm::vec2(v1), glm::vec2(v2));
}

float Vector2::Distance(const Vector2 &v1, const Vector2 &v2)
{
    return (v1 - v2).Length();
}










Vector2 operator+(const Vector2 & v1, const Vector2 &v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}


Vector2 operator*(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(v1.x * v2.x, v1.y * v2.y);
}


Vector2 operator-(const Vector2 &v)
{
    return Vector2(-v.x, -v.y);
}


Vector2 operator*(float a, const Vector2 &v)
{
    return Vector2(a * v.x, a * v.y);
}


Vector2 operator*(const Vector2 &v, float a)
{
    return a * v;
}


Vector2 operator/(float a, const Vector2 &v)
{
    return Vector2(a / v.x, a / v.y);
}


Vector2 operator/(const Vector2 &v, float a)
{
    return Vector2(v.x / a, v.y / a);
}


Vector2 operator/(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(v1.x / v2.x, v1.y / v2.y);
}


Vector2 &operator+=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}


Vector2 &operator-=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 &operator*=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}


Vector2 &operator/=(Vector2 &lhs, const Vector2 &rhs)
{
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}


Vector2 operator+(float a, const Vector2 &v)
{
    return Vector2(a + v.x, a + v.y);
}


Vector2 operator+(const Vector2 &v, float a)
{
    return a + v;
}


Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}



Vector2 operator-(float a, const Vector2 &v)
{
    return Vector2(a - v.x, a - v.y);
}


Vector2 operator-(const Vector2 &v, float a)
{
    return Vector2(v.x - a, v.y - a);
}


Vector2 &operator+=(Vector2 &lhs, float a)
{
    lhs.x += a;
    lhs.y += a;
    return lhs;
}


Vector2 &operator-=(Vector2 &lhs, float a)
{
    lhs.x -= a;
    lhs.y -= a;
    return lhs;
}


Vector2 &operator*=(Vector2 &lhs, float a)
{
    lhs.x *= a;
    lhs.y *= a;
    return lhs;
}


Vector2 &operator/=(Vector2 &lhs, float a)
{
    lhs.x /= a;
    lhs.y /= a;
    return lhs;
}
