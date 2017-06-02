#include "Bang/Quaternion.h"

#include <iostream>

#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"

#include "Bang/glm/glm.hpp"
#include "Bang/glm/gtx/transform.hpp"
#include "Bang/glm/gtc/quaternion.hpp"

Quaternion Quaternion::Identity = Quaternion();

Quaternion::Quaternion() : glm::quat()
{
}

Quaternion::Quaternion(const glm::quat &q) : glm::quat(q)
{
}

Quaternion::Quaternion(float w, float x, float y, float z) : glm::quat(w,x,y,z)
{
}

Quaternion Quaternion::Conjugated() const
{
    return Quaternion(glm::conjugate(glm::quat(*this)));
}

Quaternion Quaternion::Normalized() const
{
    return Quaternion(glm::normalize(glm::quat(*this)));
}

Quaternion Quaternion::Inversed() const
{
    return Quaternion(glm::inverse(glm::quat(*this)));
}

Vector3 Quaternion::EulerAngles() const
{
    return Quaternion::EulerAngles(*this);
}

String Quaternion::ToString() const
{
    return "(" + String::ToString(x) + ", " +
                 String::ToString(y) + ", " +
                 String::ToString(z) + ", " +
                 String::ToString(w) + ")";
}

Quaternion Quaternion::Slerp(const Quaternion &from, const Quaternion &to, float progression)
{
    return Quaternion(glm::mix(glm::quat(from), glm::quat(to), progression));
}

Quaternion Quaternion::FromTo(const Vector3 &from, const Vector3 &to)
{
    Vector3 v0 = from.Normalized();
    Vector3 v1 = to.Normalized();

    const float d = Vector3::Dot(v0, v1);
    if (d >= 1.0f)
    {
        return Quaternion();
    }
    else if (d <= -1.0f) // exactly opposite
    {
        Vector3 axis(1,0,0);
        axis = Vector3::Cross(axis, v0);
        if (axis.Length() == 0)
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

Quaternion Quaternion::LookDirection(const Vector3 &_forward,
                                     const Vector3 &_up)
{
    Vector3 forward = _forward.Normalized();
    Vector3 up      = _up.Normalized();
/*
    Vector3::OrthoNormalize(up, forward);
    Vector3 right = Vector3::Cross(up, forward);

    Quaternion ret;
    ret.w = Math::Sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
    float w4_recip = 1.0f / (4.0f * ret.w);
    ret.x = (up.z - forward.y) * w4_recip;
    ret.y = (forward.x - right.z) * w4_recip;
    ret.z = (right.y - up.x) * w4_recip;
    return ret;
*/
    if (Vector3::Dot(forward,  up) >= 0.99f ||
        Vector3::Dot(forward, -up) >= 0.99f)
    {
        std::cerr << "LookDirection: Forward and up aligned. Returning identity"
                  << std::endl;
        return Quaternion::Identity;
    }

    Vector3 eye(0.0f);
    return Quaternion( glm::quat_cast(
                           glm::inverse( glm::lookAt(eye, forward, up)) ) );
}

Vector3 Quaternion::EulerAngles(const Quaternion &q)
{
    return Vector3(glm::eulerAngles(q));
}

Quaternion Quaternion::AngleAxis(float angleRads, const Vector3 &axis)
{
    glm::quat q = glm::angleAxis(angleRads, axis);
    return Quaternion(q);
}

Quaternion Quaternion::FromRotationMatrix(const Matrix4 &rm)
{
    return Quaternion(glm::quat_cast(rm.ToGlmMat4()));
}


Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
    return Quaternion(glm::quat(q1) * glm::quat(q2));
}
