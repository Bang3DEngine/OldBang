#ifndef COLOR_H
#define COLOR_H

#include "Bang.h"

FORWARD   class QColor;
FORWARD   class String;
FORWARD_T class Vector2G;
FORWARD_T class Vector3G;
FORWARD_T class Vector4G;

class Color
{
public:
    float r, g, b, a;

    Color();
    explicit Color(float m);
    explicit Color(const Vector2G<float> &v, float b = 0, float a = 1);
    explicit Color(const Vector3G<float> &v, float a = 1);
    explicit Color(const Vector4G<float> &v);
    explicit Color(float r, float g, float b);
    explicit Color(float r, float g, float b, float a);
    explicit Color(const Color &c, float a);

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
    Vector3G<float> ToVector3() const;
    Vector4G<float> ToVector4() const;

    void Ceil();

    static Color FromVector3(const Vector3G<float> &v);
    static Color FromVector4(const Vector4G<float> &v);

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
