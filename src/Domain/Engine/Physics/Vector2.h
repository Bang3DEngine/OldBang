#ifndef VECTOR2_H
#define VECTOR2_H

#include "glm/glm.hpp"

class String;
class Matrix4;
class Vector3;
class Vector2 : public glm::vec2
{
public:
    Vector2();
    explicit Vector2(const glm::vec2 &v);
    Vector2(float a);
    Vector2(float x, float y);

    /**
     * @brief Returns the length/magnitude of this Vector
     */
    float Length() const;

    /**
     * @brief Normalizes this Vector
     */
    void Normalize();

    /**
     * @brief Returns this Vector Normalized
     * @return
     */
    Vector2 Normalized() const;

    /**
     * @brief Returns this Vector with a rad->degrees conversion to all its components
     * @return
     */
    Vector2 ToDegrees() const;

    /**
     * @brief Returns this Vector with a degrees->rad conversion to all its components
     * @return
     */
    Vector2 ToRadians() const;

    glm::vec2 ToGlmVec2() const;
    String ToString() const;

    float Distance(const Vector2 &p) const;

    /**
     * @brief If progression == 0, returns v1.
     *        If progression == 1, returns v2.
     *        If 0 < progression < 1, returns a linear interpolation between v1 and v2.
     * @param v1 First Vector
     * @param v2 Second Vector
     * @param v2 A float between 0 and 1 indicating the progression.
     * @return
     */
    static Vector2 Lerp(const Vector2 &v1,
                        const Vector2 &v2,
                        float progression);

    Vector2 Abs() const;

    static Vector2 Abs(const Vector2 &v);
    static float Dot(const Vector2 &v1, const Vector2 &v2);
    static float Distance(const Vector2 &v1, const Vector2 &v2);

    const static Vector2 up;
    const static Vector2 down;
    const static Vector2 right;
    const static Vector2 left;
    const static Vector2 zero;
    const static Vector2 one;
};

Vector2 operator+(float a, const Vector2& v);
Vector2 operator+(const Vector2& v, float a);
Vector2 operator+(const Vector2& v1, const Vector2& v2);

Vector2 operator-(float a, const Vector2& v);
Vector2 operator-(const Vector2& v, float a);
Vector2 operator-(const Vector2& v1, const Vector2& v2);
Vector2 operator-(const Vector2& v);

Vector2 operator*(float a, const Vector2& v);
Vector2 operator*(const Vector2& v, float a);
Vector2 operator*(const Vector2& v1, const Vector2& v2);

Vector2 operator/(float a, const Vector2& v);
Vector2 operator/(const Vector2& v, float a);
Vector2 operator/(const Vector2& v1, const Vector2& v2);
Vector2 operator*(const Matrix4& m, const Vector2& v);

Vector2& operator+=(Vector2& lhs, const Vector2& rhs);
Vector2& operator-=(Vector2& lhs, const Vector2& rhs);
Vector2& operator*=(Vector2& lhs, const Vector2& rhs);
Vector2& operator/=(Vector2& lhs, const Vector2& rhs);
Vector2& operator+=(Vector2& lhs, float a);
Vector2& operator-=(Vector2& lhs, float a);
Vector2& operator*=(Vector2& lhs, float a);
Vector2& operator/=(Vector2& lhs, float a);

#endif // VECTOR2_H
