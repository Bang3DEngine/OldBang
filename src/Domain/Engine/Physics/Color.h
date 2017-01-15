#ifndef COLOR_H
#define COLOR_H

#include <QColor>

class String;
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

    String ToStringRgb() const;
    String ToStringRgb255() const;
    String ToStringRgba() const;
    String ToStringRgba255() const;

    static Color FromQColor(const QColor &c);
    QColor ToQColor() const;
    String ToString() const;
    Vector3 ToVector3() const;
    Vector4 ToVector4() const;

    void Ceil();

    static Color FromVector3(const Vector3 &v);
    static Color FromVector4(const Vector4 &v);

    const static Color Red;
    const static Color Orange;
    const static Color Yellow;
    const static Color Green;
    const static Color Turquoise;
    const static Color LightBlue;
    const static Color Blue;
    const static Color DarkBlue;
    const static Color Purple;
    const static Color Pink;
    const static Color Black;
    const static Color Gray;
    const static Color White;
    const static Color Zero;
    const static Color One;
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

bool operator==(const Color& lhs, const Color& rhs);
bool operator!=(const Color& lhs, const Color& rhs);
Color& operator+=(Color& lhs, const Color& rhs);
Color& operator-=(Color& lhs, const Color& rhs);
Color& operator*=(Color& lhs, const Color& rhs);
Color& operator/=(Color& lhs, const Color& rhs);
Color& operator+=(Color& lhs, float m);
Color& operator-=(Color& lhs, float m);
Color& operator*=(Color& lhs, float m);
Color& operator/=(Color& lhs, float m);

#endif // COLOR_H
