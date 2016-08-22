#include "Color.h"

#include <sstream>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

const Color Color::red         = Color(1,     0,   0,  1);
const Color Color::orange      = Color(1,   0.5,   0,  1);
const Color Color::yellow      = Color(1,     1,   0,  1);
const Color Color::green       = Color(0,     1,   0,  1);
const Color Color::turquoise   = Color(1,     1,   0,  1);
const Color Color::blue        = Color(0,     0,   1,  1);
const Color Color::purple      = Color(0.5,   0,   1,  1);
const Color Color::pink        = Color(1,     0,   1,  1);
const Color Color::black       = Color(0,     0,   0,  1);
const Color Color::gray        = Color(0.5, 0.5, 0.5,  1);
const Color Color::white       = Color(1,     1,   1,  1);
const Color Color::zero        = Color(0,     0,   0,  0);
const Color Color::one         = Color(1,     1,   1,  1);

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

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(float r, float g, float b, float a) : Color(r, g, b)
{
    this->a = a;
}

Color::Color(const Color &c, float a) : Color(c.r, c.g, c.b, a)
{
}

Color Color::Lerp(const Color &c1,
                  const Color &c2,
                  float progression)
{
    return Color(Vector4(
                 glm::mix(glm::vec4(c1.r, c1.g, c1.b, c1.a),
                          glm::vec4(c2.r, c2.g, c2.b, c2.a), progression)));
}

std::string Color::ToStringRgb() const
{
    std::ostringstream oss;
    oss << "(" << r << ", " << g << ", " << b << ")";
    return oss.str();
}

std::string Color::ToStringRgb255() const
{
    std::ostringstream oss;
    oss << "(" << int(r * 255) << ", " <<
                  int(g * 255) << ", " <<
                  int(b * 255) << ")";
    return oss.str();
}

std::string Color::ToStringRgba() const
{
    std::ostringstream oss;
    oss << "(" << r << ", " << g << ", " << b << ", " << a << ")";
    return oss.str();
}

std::string Color::ToStringRgba255() const
{
    std::ostringstream oss;
    oss << "(" << int(r * 255) << ", " <<
                  int(g * 255) << ", " <<
                  int(b * 255) << ", " <<
                  int(a * 255) << ")";
    return oss.str();
}

Color Color::FromQColor(const QColor &c)
{
    return Color(c.red()   / 255.0f,
                 c.green() / 255.0f,
                 c.blue()  / 255.0f,
                 c.alpha() / 255.0f);
}

QColor Color::ToQColor() const
{
    return QColor(r * 255, g * 255, b * 255, a * 255);
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
    r = glm::ceil(r);
    g = glm::ceil(g);
    b = glm::ceil(b);
    a = glm::ceil(a);
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


Color operator*(float m, const Color &c)
{
    return Color(m * c.r, m * c.g, m * c.b, m * c.a);
}


Color operator*(const Color &c, float m)
{
    return m * c;
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
