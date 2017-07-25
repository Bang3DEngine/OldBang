#include "Bang/Quaternion.h"

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"

Quaternion Quaternion::Identity = Quaternion();

Quaternion::Quaternion() : x(0), y(0), z(0), w(1)
{
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) :
    x(_x), y(_y), z(_z), w(_w)
{
}

float Quaternion::Length() const
{
    return Math::Sqrt(SqLength());
}

float Quaternion::SqLength() const
{
    return (x*x + y*y + z*z + w*w);
}

Quaternion Quaternion::Conjugated() const
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Normalized() const
{
    float length = Length();
    return Quaternion(x/length, y/length, z/length, w/length);
}

Quaternion Quaternion::Inversed() const
{
    return Conjugated()/Quaternion::Dot(*this, *this);
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

float Quaternion::Pitch() const
{
    return SCAST<float>(
                Math::ATan2(
                    (SCAST<float>(2) * (y * z + w * x)),
                    (w * w - x * x - y * y + z * z)
                )
           );
}

float Quaternion::Yaw() const
{
    return Math::ASin(
                Math::Clamp(
                    SCAST<float>(-2) * (x * z - w * y),
                    SCAST<float>(-1),
                    SCAST<float>(1)
                )
            );
}

float Quaternion::Roll() const
{
    return SCAST<float>(
                Math::ATan2(
                    (SCAST<float>(2) * (x * y + w * z)),
                    (w * w + x * x - y * y - z * z)
                )
           );
}

float Quaternion::Dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
}

Quaternion Quaternion::Cross(const Quaternion &q1, const Quaternion &q2)
{
    return Quaternion(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
                      q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
                      q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x,
                      q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}

Quaternion Quaternion::Lerp(const Quaternion &from,
                            const Quaternion &to,
                            float t)
{
    float cosTheta = Quaternion::Dot(from, to);

    if(cosTheta > SCAST<float>(1) - SCAST<float>(0.01))
    {
        return Quaternion(Math::Lerp(from.x, to.x, t),
                          Math::Lerp(from.y, to.y, t),
                          Math::Lerp(from.z, to.z, t),
                          Math::Lerp(from.w, to.w, t));
    }
    else
    {
        float angle = Math::ACos(cosTheta);
        return (
                 Math::Sin((SCAST<float>(1) - t) * angle) * from +
                 Math::Sin(t * angle) * to
                ) /
                Math::Sin(angle);
    }
}

Quaternion Quaternion::SLerp(const Quaternion &from,
                             const Quaternion &_to,
                             float t)
{
    Quaternion to = _to;

    float cosTheta = Quaternion::Dot(from, _to);

    // If cosTheta < 0, the interpolation will take the long way around the sphere.
    // To fix this, one quat must be negated.
    if (cosTheta < SCAST<float>(0))
    {
        to       = -_to;
        cosTheta = -cosTheta;
    }

    if(cosTheta > SCAST<float>(1) - SCAST<float>(0.01))
    {
        return Quaternion( Math::Lerp(from.x, to.x, t),
                           Math::Lerp(from.y, to.y, t),
                           Math::Lerp(from.z, to.z, t),
                           Math::Lerp(from.w, to.w, t));
    }
    else
    {
        float angle = Math::ACos(cosTheta);
        return (
                    Math::Sin((SCAST<float>(1) - t) * angle) * from +
                    Math::Sin(t * angle) * to
                ) /
                Math::Sin(angle);
    }
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
    else if (d <= -1.0f)
    {
        Vector3 axis(1,0,0);
        axis = Vector3::Cross(axis, v0);
        if (axis.Length() == 0)
        {
            axis = Vector3(0, 1, 0);
            axis = Vector3::Cross(axis, v0);
        }

        return Quaternion(0.0f, axis.x, axis.y, axis.z).Normalized();
    }

    const float s = float(Math::Sqrt( (1+d)*2 ));
    const float invs = 1.0f/s;
    const Vector3 c = Vector3::Cross(v0, v1) * invs;
    return Quaternion(s * 0.5f, c.x, c.y, c.z).Normalized();
}

Quaternion Quaternion::LookDirection(const Vector3 &_forward,
                                     const Vector3 &_up)
{
    Vector3 forward = _forward.Normalized();
    Vector3 up      = _up.Normalized();

    if (Vector3::Dot(forward,  up) >= 0.99f ||
        Vector3::Dot(forward, -up) >= 0.99f)
    {
        Debug_Warn("LookDirection: Forward and up aligned. Returning identity");
        return Quaternion::Identity;
    }
    return Matrix4::ToQuaternion(
                Matrix4::LookAt(Vector3::Zero, forward, up).Inversed());
}

Vector3 Quaternion::EulerAngles(const Quaternion &q)
{
    return Vector3(q.Pitch(), q.Yaw(), q.Roll());
}

Quaternion Quaternion::AngleAxis(float angleRads, const Vector3 &axis)
{
    const float s = Math::Sin(angleRads * SCAST<float>(0.5));
    return Quaternion(axis.x * s,
                      axis.y * s,
                      axis.z * s,
                      Math::Cos(angleRads * SCAST<float>(0.5)));
}


Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
    Quaternion res = q1;
    res *= q2;
    return res;
}

Quaternion &operator*=(Quaternion &lhs, const Quaternion &rhs)
{
    const Quaternion p(lhs);
    const Quaternion q(rhs);

    lhs.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
    lhs.y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
    lhs.z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
    lhs.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
    return lhs;
}

Quaternion operator*(const Quaternion &q, float a)
{
    return Quaternion(q.x*a, q.y*a, q.z*a, q.w*a);
}
Quaternion operator*(float a, const Quaternion &q) { return q * a; }

Quaternion operator/(const Quaternion &q, float a)
{
    return Quaternion(q.x/a, q.y/a, q.z/a, q.w/a);
}
Quaternion operator/(float a, const Quaternion &q)
{
    return Quaternion(a/q.x, a/q.y, a/q.z, a/q.w);
}

Quaternion operator-(const Quaternion &q)
{
    return Quaternion(-q.x, -q.y, -q.z, -q.w);
}

Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
    Quaternion res = q1;
    res += q2;
    return res;
}

Quaternion &operator+=(Quaternion &lhs, const Quaternion &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}
