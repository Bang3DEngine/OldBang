#ifndef COLOR_H
#define COLOR_H

#include <QColor>

#include "glm/glm.hpp"

class Vector2;
class Vector3;
class Vector4;
class Color
{
public:
    float r, g, b, a;

    Color();
    Color(float m);
    Color(const Vector2 &v, float b = 0, float a = 1);
    Color(const Vector3 &v, float a = 1);
    Color(const Vector4 &v);
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const Color &c, float a);

    /**
     * @brief If progression == 0, returns v1.
     *        If progression == 1, returns v2.
     *        If 0 < progression < 1, returns a linear interpolation between v1 and v2.
     * @param v1 First Color
     * @param v2 Second Color
     * @param v2 A float between 0 and 1 indicating the progression.
     * @return
     */
    static Color Lerp(const Color &v1,
                      const Color &v2,
                      float progression);

    std::string ToStringRgb() const;
    std::string ToStringRgb255() const;
    std::string ToStringRgba() const;
    std::string ToStringRgba255() const;

    static Color FromQColor(const QColor &c);
    QColor ToQColor() const;
    Vector3 ToVector3() const;

    const static Color red;
    const static Color orange;
    const static Color yellow;
    const static Color green;
    const static Color turquoise;
    const static Color blue;
    const static Color purple;
    const static Color pink;
    const static Color black;
    const static Color gray;
    const static Color white;
    const static Color zero;
    const static Color one;
};

Color operator+(float m, const Color& v);
Color operator+(const Color& v, float m);
Color operator+(const Color& v1, const Color& v2);

Color operator-(float m, const Color& v);
Color operator-(const Color& v, float m);
Color operator-(const Color& v1, const Color& v2);
Color operator-(const Color& v);

Color operator*(float m, const Color& v);
Color operator*(const Color& v, float m);
Color operator*(const Color& v1, const Color& v2);

Color operator/(float m, const Color& v);
Color operator/(const Color& v, float m);
Color operator/(const Color& v1, const Color& v2);

Color& operator+=(Color& lhs, const Color& rhs);
Color& operator-=(Color& lhs, const Color& rhs);
Color& operator*=(Color& lhs, const Color& rhs);
Color& operator/=(Color& lhs, const Color& rhs);
Color& operator+=(Color& lhs, float m);
Color& operator-=(Color& lhs, float m);
Color& operator*=(Color& lhs, float m);
Color& operator/=(Color& lhs, float m);

#endif // COLOR_H
