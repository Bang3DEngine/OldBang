#ifndef VECTOR3_H
#define VECTOR3_H

#include "glm/glm.hpp"

class Quaternion;
class Vector3 : public glm::vec3
{
public:
    Vector3();
    explicit Vector3(const glm::vec3 &v);
    Vector3(float a);
    Vector3(float x, float y, float z);

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
    Vector3 Normalized() const;

    /**
     * @brief Returns this Vector with a rad->degrees conversion to all its components
     * @return
     */
    Vector3 ToDegrees() const;

    /**
     * @brief Returns this Vector with a degrees->rad conversion to all its components
     * @return
     */
    Vector3 ToRadians() const;

    glm::vec3 ToGlmVec3() const;

    float Distance(const Vector3 &p) const;

    /**
     * @brief Makes v1 and v2 orthogonal and normalizes them.
     * @param v1 First Vector
     * @param v2 Second Vector
     */
    static void OrthoNormalize(Vector3 &v1, Vector3 &v2);

    /**
     * @brief If progression == 0, returns v1.
     *        If progression == 1, returns v2.
     *        If 0 < progression < 1, returns a linear interpolation between v1 and v2.
     * @param v1 First Vector
     * @param v2 Second Vector
     * @param v2 A float between 0 and 1 indicating the progression.
     * @return
     */
    static Vector3 Lerp(const Vector3 &v1,
                        const Vector3 &v2,
                        float progression);

    Vector3 Abs() const;

    static Vector3 Abs(const Vector3 &v);
    static Vector3 Cross(const Vector3 &v1, const Vector3 &v2);
    static float Dot(const Vector3 &v1, const Vector3 &v2);
    static float Distance(const Vector3 &v1, const Vector3 &v2);

    const static Vector3 up;
    const static Vector3 down;
    const static Vector3 right;
    const static Vector3 left;
    const static Vector3 forward;
    const static Vector3 back;
    const static Vector3 zero;
    const static Vector3 one;
};

Vector3 operator+(float a, const Vector3& v);
Vector3 operator+(const Vector3& v, float a);
Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v);

Vector3 operator*(Quaternion q, const Vector3& rhs);
Vector3 operator*(float a, const Vector3& v);
Vector3 operator*(const Vector3& v, float a);
Vector3 operator*(const Vector3& v1, const Vector3& v2);

Vector3 operator/(float a, const Vector3& v);
Vector3 operator/(const Vector3& v, float a);
Vector3 operator/(const Vector3& v1, const Vector3& v2);

Vector3& operator+=(Vector3& lhs, const Vector3& rhs);
Vector3& operator-=(Vector3& lhs, const Vector3& rhs);
Vector3& operator*=(Vector3& lhs, const Vector3& rhs);
Vector3& operator/=(Vector3& lhs, const Vector3& rhs);

#endif // VECTOR3_H
