#include "Bang/Color.h"

#include <sstream>

#include "Bang/Math.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

NAMESPACE_BANG_BEGIN

const Color Color::Red         = Color(1,     0,   0,  1);
const Color Color::Orange      = Color(1,   0.5,   0,  1);
const Color Color::Yellow      = Color(1,     1,   0,  1);
const Color Color::Green       = Color(0,     1,   0,  1);
const Color Color::Turquoise   = Color(1,     1,   0,  1);
const Color Color::LightBlue   = Color(0.2, 0.5,   1,  1);
const Color Color::Blue        = Color(0,     0,   1,  1);
const Color Color::DarkBlue    = Color(0,     0, 0.6,  1);
const Color Color::Purple      = Color(0.5,   0,   1,  1);
const Color Color::Pink        = Color(1,     0,   1,  1);
const Color Color::Black       = Color(0,     0,   0,  1);
const Color Color::LightGray   = Color(0.8, 0.8, 0.8,  1);
const Color Color::Gray        = Color(0.5, 0.5, 0.5,  1);
const Color Color::White       = Color(1,     1,   1,  1);
const Color Color::Zero        = Color(0,     0,   0,  0);
const Color Color::One         = Color(1,     1,   1,  1);

Color::Color() : Color(0)
{
}

Color::Color(float m) : Color(m, m, m, m)
{
}

Color::Color(const Vector2 &v, float b, float a) : Color(v.x, v.y, b, a)
{
}

Color::Color(const Vector3 &v, float a) : Color(v.x, v.y, v.z, a)
{
}

Color::Color(const Vector4 &v) : Color(v.x, v.y, v.z, v.w)
{
}

Color::Color(float r, float g, float b)  : Color(r, g, b, 1)
{
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Color(const Color &c, float a) : Color(c.r, c.g, c.b, a)
{
}

Color Color::Lerp(const Color &c1,
                  const Color &c2,
                  float t)
{
    return Color(
             Vector4::Lerp(
                     Vector4(c1.r, c1.g, c1.b, c1.a),
                     Vector4(c2.r, c2.g, c2.b, c2.a),
                     t
                    )
                );
}

String Color::ToStringRgb() const
{
    ::std::ostringstream oss;
    oss << "(" << r << ", " << g << ", " << b << ")";
    return oss.str();
}

String Color::ToStringRgb255() const
{
    ::std::ostringstream oss;
    oss << "(" << int(r * 255) << ", " <<
                  int(g * 255) << ", " <<
                  int(b * 255) << ")";
    return oss.str();
}

String Color::ToStringRgba() const
{
    ::std::ostringstream oss;
    oss << "(" << r << ", " << g << ", " << b << ", " << a << ")";
    return oss.str();
}

String Color::ToStringRgba255() const
{
    ::std::ostringstream oss;
    oss << "(" << int(r * 255) << ", " <<
                  int(g * 255) << ", " <<
                  int(b * 255) << ", " <<
                  int(a * 255) << ")";
    return oss.str();
}

String Color::ToString() const
{
    return "(" + String(r) + ", " +
                 String(g) + ", " +
                 String(b) + ", " +
                 String(a) + ")";
}

Vector3 Color::ToVector3() const
{
    return Vector3(r, g, b);
}

Vector4 Color::ToVector4() const
{
    return Vector4(r, g, b, a);
}

void Color::Ceil()
{
    r = Math::Ceil(r);
    g = Math::Ceil(g);
    b = Math::Ceil(b);
    a = Math::Ceil(a);
}

Color Color::FromVector3(const Vector3 &v)
{
    return Color(v.x, v.y, v.z, 1);
}

Color Color::FromVector4(const Vector4 &v)
{
    return Color(v.x, v.y, v.z, v.w);
}

Color operator+(const Color & c1, const Color &c2)
{
    return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a * c2.a);
}

Color operator*(const Color &c1, const Color &c2)
{
    return Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b, c1.a * c2.a);
}

Color operator-(const Color &c)
{
    return Color(-c.r, -c.g, -c.b, -c.a);
}

Color operator*(const Color &c, float m)
{
    return m * c;
}

Color operator*(float m, const Color &c)
{
    return Color(m * c.r, m * c.g, m * c.b, m * c.a);
}

Color operator/(float m, const Color &c)
{
    return Color(m / c.r, m / c.g, m / c.b, m / c.a);
}

Color operator/(const Color &c, float m)
{
    return Color(c.r / m, c.g / m, c.b / m, c.a / m);
}

Color operator/(const Color &c1, const Color &c2)
{
    return Color(c1.r / c2.r, c1.g / c2.g, c1.b / c2.b, c1.a / c2.a);
}

Color &operator+=(Color &lhs, const Color &rhs)
{
    lhs.r += rhs.r;
    lhs.g += rhs.g;
    lhs.b += rhs.b;
    lhs.a += rhs.a;
    return lhs;
}

Color &operator-=(Color &lhs, const Color &rhs)
{
    lhs.r -= rhs.r;
    lhs.g -= rhs.g;
    lhs.b -= rhs.b;
    lhs.a -= rhs.a;
    return lhs;
}

Color &operator*=(Color &lhs, const Color &rhs)
{
    lhs.r *= rhs.r;
    lhs.g *= rhs.g;
    lhs.b *= rhs.b;
    lhs.a *= rhs.a;
    return lhs;
}

Color &operator/=(Color &lhs, const Color &rhs)
{
    lhs.r /= rhs.r;
    lhs.g /= rhs.g;
    lhs.b /= rhs.b;
    lhs.a /= rhs.a;
    return lhs;
}

Color operator+(float m, const Color &c)
{
    return Color(m + c.r, m + c.g, m + c.b, m + c.a);
}

Color operator+(const Color &c, float m)
{
    return m + c;
}

Color operator-(const Color &c1, const Color &c2)
{
    return Color(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b, c1.a - c2.a);
}

Color operator-(float m, const Color &c)
{
    return Color(m - c.r, m - c.g, m - c.b, m - c.a);
}

Color operator-(const Color &c, float m)
{
    return Color(c.r - m, c.g - m, c.b - m, c.a - m);
}

Color &operator+=(Color &lhs, float m)
{
    lhs.r += m;
    lhs.g += m;
    lhs.b += m;
    lhs.a += m;
    return lhs;
}

Color &operator-=(Color &lhs, float m)
{
    lhs.r -= m;
    lhs.g -= m;
    lhs.b -= m;
    lhs.a -= m;
    return lhs;
}

Color &operator*=(Color &lhs, float m)
{
    lhs.r *= m;
    lhs.g *= m;
    lhs.b *= m;
    lhs.a *= m;
    return lhs;
}

Color &operator/=(Color &lhs, float m)
{
    lhs.r /= m;
    lhs.g /= m;
    lhs.b /= m;
    lhs.a /= m;
    return lhs;
}

bool operator==(const Color &lhs, const Color &rhs)
{
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b &&
           lhs.a == rhs.a;
}

bool operator!=(const Color &lhs, const Color &rhs)
{
    return !(lhs == rhs);
}

NAMESPACE_BANG_END
