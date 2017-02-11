#ifndef VECTOR4_H
#define VECTOR4_H

#include "glm/glm.hpp"

class Color;
class String;
class Matrix4;
class Vector2;
class Vector3;
class Quaternion;
class Vector4 : public glm::vec4
{
public:
    Vector4();
    explicit Vector4(const glm::vec4 &v);
    explicit Vector4(float a);
    explicit Vector4(float x, float y, float z, float w);
    explicit Vector4(const Color &c);
    explicit Vector4(const Vector3 &v, float w);
    explicit Vector4(const Vector2 &v, float z, float w);

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
    Vector4 Normalized() const;

    /**
     * @brief Returns this Vector with a rad->degrees conversion to all its components
     * @return
     */
    Vector4 ToDegrees() const;

    /**
     * @brief Returns this Vector with a degrees->rad conversion to all its components
     * @return
     */
    Vector4 ToRadians() const;

    glm::vec4 ToGlmVec4() const;
    String ToString() const;

    float Distance(const Vector4 &p) const;

    /**
     * @brief If progression == 0, returns v1.
     *        If progression == 1, returns v2.
     *        If 0 < progression < 1, returns a linear interpolation between v1 and v2.
     * @param v1 First Vector
     * @param v2 Second Vector
     * @param v2 A float between 0 and 1 indicating the progression.
     * @return
     */
    static Vector4 Lerp(const Vector4 &v1,
                        const Vector4 &v2,
                        float progression);

    Vector4 Abs() const;

    float* Values() const;

    static Vector4 Abs(const Vector4 &v);
    static float Dot(const Vector4 &v1, const Vector4 &v2);
    static float Distance(const Vector4 &v1, const Vector4 &v2);

    const static Vector4 Up;
    const static Vector4 Down;
    const static Vector4 Right;
    const static Vector4 Left;
    const static Vector4 Forward;
    const static Vector4 Back;
    const static Vector4 Zero;
    const static Vector4 One;

    // SWIZZLING
    Vector2  xy() const;
    Vector3  xyz() const;
    //
};

Vector4 operator+(float a, const Vector4& v);
Vector4 operator+(const Vector4& v, float a);
Vector4 operator+(const Vector4& v1, const Vector4& v2);

Vector4 operator-(float a, const Vector4& v);
Vector4 operator-(const Vector4& v, float a);
Vector4 operator-(const Vector4& v1, const Vector4& v2);
Vector4 operator-(const Vector4& v);

Vector4 operator*(Quaternion q, const Vector4& rhs);
Vector4 operator*(float a, const Vector4& v);
Vector4 operator*(const Vector4& v, float a);
Vector4 operator*(const Vector4& v1, const Vector4& v2);

Vector4 operator/(float a, const Vector4& v);
Vector4 operator/(const Vector4& v, float a);
Vector4 operator/(const Vector4& v1, const Vector4& v2);
Vector4 operator*(const Matrix4& m, const Vector4& v);

Vector4& operator+=(Vector4& lhs, const Vector4& rhs);
Vector4& operator-=(Vector4& lhs, const Vector4& rhs);
Vector4& operator*=(Vector4& lhs, const Vector4& rhs);
Vector4& operator/=(Vector4& lhs, const Vector4& rhs);
Vector4& operator+=(Vector4& lhs, float a);
Vector4& operator-=(Vector4& lhs, float a);
Vector4& operator*=(Vector4& lhs, float a);
Vector4& operator/=(Vector4& lhs, float a);

#endif // VECTOR4_H
