#ifndef QUATERNION_H
#define QUATERNION_H


#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

class Vector3;
class Quaternion : public glm::quat
{
public:
    Quaternion();
    explicit Quaternion(const glm::quat &q);
    Quaternion(float w, float x, float y, float z);

    Quaternion Normalized() const;
    Quaternion Inversed() const;

    static Vector3 EulerAngles(const Quaternion &q);
    static Quaternion Slerp(const Quaternion &from,
                            const Quaternion &to,
                            float progression);
    static Quaternion FromTo(const Vector3 &from, const Vector3 &to);
    static Quaternion LookDirection(const Vector3 &dir);
    static Quaternion AngleAxis(float angleDeg, const Vector3 &axis);
};


Quaternion operator*(const Quaternion &q1, const Quaternion& q2);

#endif // QUATERNION_H
