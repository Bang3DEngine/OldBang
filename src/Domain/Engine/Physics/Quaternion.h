#ifndef QUATERNION_H
#define QUATERNION_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Vector3.h"

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
    static Quaternion AngleAxis(float angleDeg, const Vector3 &axis);
    static Quaternion FromRotationMatrix(const Matrix4 &rotMatrix);

    static Quaternion Identity;
};

Quaternion operator*(const Quaternion &q1, const Quaternion& q2);

#endif // QUATERNION_H
