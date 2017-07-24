#ifndef QUATERNION_H
#define QUATERNION_H

#include "Bang/glm/glm.hpp"
#include "Bang/glm/gtx/transform.hpp"
#include "Bang/glm/gtc/quaternion.hpp"

#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

class String;
class Matrix4;
class Quaternion : public glm::quat
{
public:
    Quaternion();
    explicit Quaternion(const glm::quat &q);
    Quaternion(float w, float x, float y, float z);

    Quaternion Conjugated() const;
    Quaternion Normalized() const;
    Quaternion Inversed() const;

    Vector3 EulerAngles() const;
    String ToString() const;

    static Vector3 EulerAngles(const Quaternion &q);
    static Quaternion Slerp(const Quaternion &from,
                            const Quaternion &to,
                            float progression);
    static Quaternion FromTo(const Vector3 &from, const Vector3 &to);
    static Quaternion LookDirection(const Vector3 &_forward,
                                    const Vector3 &_up = Vector3::Up);
    static Quaternion AngleAxis(float angleRads, const Vector3 &axis);
    static Quaternion FromRotationMatrix(const Matrix4 &rotMatrix);

    static Quaternion Identity;
};

Quaternion operator*(const Quaternion &q1, const Quaternion& q2);

template<class T>
Vector4G<T> operator*(Quaternion q, const Vector4G<T> &rhs)
{
    return Vector4G<T>(q * glm::vec4(rhs.x, rhs.y, rhs.z, rhs.w));
}

template<class T>
Vector3G<T> operator*(const Quaternion& q, const Vector3G<T>& rhs)
{
    glm::vec3 res = glm::quat(q.x, q.y, q.z, q.w) *
                    glm::vec3(rhs.x, rhs.y, rhs.z);
    return Vector3G<T>(res.x, res.y, res.z);
}

template<class T>
Vector3G<T> operator*(const Vector3G<T>& lhs, const Quaternion& q)
{
    glm::vec3 res = glm::vec3(lhs.x, lhs.y, lhs.z) *
                    glm::quat(q.x, q.y, q.z, q.w);
    return Vector3G<T>(res.x, res.y, res.z);
}

#endif // QUATERNION_H
