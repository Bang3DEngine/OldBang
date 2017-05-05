#include "Bang/Vector2.h"

#include "Bang/Math.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"

const Vector2 Vector2::Up      = Vector2( 0, 1);
const Vector2 Vector2::Down    = Vector2( 0,-1);
const Vector2 Vector2::Right   = Vector2( 1, 0);
const Vector2 Vector2::Left    = Vector2(-1, 0);
const Vector2 Vector2::Zero    = Vector2( 0, 0);
const Vector2 Vector2::One     = Vector2( 1, 1);

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
    return float(Math::Sqrt(x*x + y*y));
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
    return Vector2( Math::Rad2Deg(x), Math::Rad2Deg(y) );
}

Vector2 Vector2::ToRadians() const
{
    return Vector2( Math::Deg2Rad(x), Math::Deg2Rad(y) );
}

glm::vec2 Vector2::ToGlmVec2() const
{
    return glm::vec2(x,y);
}

String Vector2::ToString() const
{
    return "(" + String::ToString(x) + ", " +
                 String::ToString(y) + ")";
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
    return Vector2(Math::Abs(x), Math::Abs(y));
}

float *Vector2::Values() const
{
    return (float*) &x;
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

Vector2 Vector2::Max(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(Math::Max(v1.x, v2.x),
                   Math::Max(v1.y, v2.y));
}

Vector2 Vector2::Min(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(Math::Min(v1.x, v2.x),
                   Math::Min(v1.y, v2.y));
}

Vector2 Vector2::Clamp(const Vector2 &v, const Vector2 &min, const Vector2 &max)
{
    return Vector2::Min( Vector2::Max(v, max), min );
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
