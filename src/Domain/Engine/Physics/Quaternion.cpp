#include "Quaternion.h"

#include "Vector3.h"

Quaternion::Quaternion() : glm::quat()
{
}

Quaternion::Quaternion(const glm::quat &q) : glm::quat(q)
{
}

Quaternion::Quaternion(float w, float x, float y, float z) : glm::quat(w,x,y,z)
{
}

Quaternion Quaternion::Normalized() const
{
    return Quaternion(glm::normalize(glm::quat(*this)));
}

Vector3 Quaternion::EulerAngles(const Quaternion &q)
{
    return Vector3(glm::eulerAngles(q));
}

Quaternion Quaternion::AngleAxis(float angleDeg, const Vector3 &axis)
{
    glm::quat q = glm::angleAxis(angleDeg, axis);
    return Quaternion(q);
}


Quaternion operator*(Quaternion q1, const Quaternion &q2)
{
    return Quaternion(glm::quat(q1) * glm::quat(q2));
}
