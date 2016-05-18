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

Quaternion Quaternion::Lerp(const Quaternion &from, const Quaternion &to, float progression)
{
    return Quaternion(glm::mix(glm::quat(from), glm::quat(to), progression));
}

Quaternion Quaternion::FromTo(const Vector3 &from, const Vector3 &to)
{
    Vector3 v0 = from.Normalized();
    Vector3 v1 = to.Normalized();

    const float d = Vector3::Dot(v0, v1);
    if(d >= 1.0f)
    {
        return Quaternion();
    }
    else if(d <= -1.0f) // exactly opposite
    {
        Vector3 axis(1,0,0);
        axis = Vector3::Cross(axis, v0);
        if(axis.Length() == 0)
        {
            axis = Vector3(0,1,0);
            axis = Vector3::Cross(axis, v0);
        }
        // same as fromAngleAxis(core::PI, axis).normalize();
        return Quaternion(0.0f, axis.x, axis.y, axis.z).Normalized();
    }

    const float s = float(std::sqrt( (1+d)*2 )); // optimize inv_sqrt
    const float invs = 1.0f / s;
    const Vector3 c = Vector3::Cross(v0, v1) * invs;
    return Quaternion(s * 0.5f, c.x, c.y, c.z).Normalized();
}

Quaternion Quaternion::LookAt(const Vector3 &dir)
{
    return FromTo(Vector3::forward, dir);
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


Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
    return Quaternion(glm::quat(q1) * glm::quat(q2));
}
