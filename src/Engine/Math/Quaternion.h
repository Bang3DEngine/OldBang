#ifndef QUATERNION_H
#define QUATERNION_H

#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

// Most of it almost copied from glm

class String;
class Quaternion
{
public:
    float x, y, z, w;

    Quaternion();
    Quaternion(float x, float y, float z, float w);

    float Length() const;
    float SqLength() const;

    Quaternion Conjugated() const;
    Quaternion Normalized() const;
    Quaternion Inversed() const;

    Vector3 EulerAngles() const;
    String ToString() const;

    float Pitch() const;
    float Yaw() const;
    float Roll() const;

    static float Dot(const Quaternion &q1, const Quaternion &q2);
    static Quaternion Cross(const Quaternion &q1, const Quaternion &q2);
    static Vector3 EulerAngles(const Quaternion &q);
    static Quaternion Lerp(const Quaternion &from,
                            const Quaternion &to,
                            float t);
    static Quaternion SLerp(const Quaternion &from,
                            const Quaternion &to,
                            float t);
    static Quaternion FromTo(const Vector3 &from, const Vector3 &to);
    static Quaternion LookDirection(const Vector3 &_forward,
                                    const Vector3 &_up = Vector3::Up);
    static Quaternion AngleAxis(float angleRads, const Vector3 &axis);

    static Quaternion Identity;
};

Quaternion operator+(const Quaternion &q1, const Quaternion &q2);
Quaternion& operator+=(Quaternion& lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &q, float a);
Quaternion operator*(float a, const Quaternion &q);
Quaternion operator/(const Quaternion &q, float a);
Quaternion operator/(float a, const Quaternion &q);
Quaternion& operator*=(Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &q1, const Quaternion& q2);
Quaternion operator-(const Quaternion &q);

template<class T>
Vector4G<T> operator*(Quaternion q, const Vector4G<T> &rhs)
{
    return Vector4G<T>(q * Vector3G<T>(rhs.x, rhs.y, rhs.w), rhs.w);
}
template<class T>
Vector4G<T> operator*(const Vector4G<T> &lhs, Quaternion q)
{
    return q.Inversed() * lhs;
}

template<class T>
Vector3G<T> operator*(const Quaternion& q, const Vector3G<T>& rhs)
{
    const Vector3G<T> qVector(q.x, q.y, q.z);
    const Vector3G<T> uv (Vector3::Cross(qVector, rhs));
    const Vector3G<T> uuv(Vector3::Cross(qVector, uv));

    return rhs + ((uv * q.w) + uuv) * SCAST<T>(2);
}
template<class T>
Vector3G<T> operator*(const Vector3G<T>& lhs, const Quaternion& q)
{
    return q.Inversed() * lhs;
}

#endif // QUATERNION_H
