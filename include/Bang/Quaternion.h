#ifndef QUATERNIONG_H
#define QUATERNIONG_H

#include "Bang/Math.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

template<class T> // Most of it almost copied from glm
class QuaternionG
{
public:
    T x, y, z, w;

    QuaternionG() : x( Cast<T>(0) ),
                    y( Cast<T>(0) ),
                    z( Cast<T>(0) ),
                    w( Cast<T>(1) )
    {
    }

    QuaternionG(T _x, T _y, T _z, T _w) :
                    x( Cast<T>(_x) ),
                    y( Cast<T>(_y) ),
                    z( Cast<T>(_z) ),
                    w( Cast<T>(_w) )
    {
    }

    T Length() const
    {
        return Math::Sqrt(SqLength());
    }
    T SqLength() const
    {
        return (x*x + y*y + z*z + w*w);
    }

    QuaternionG<T> Conjugated() const
    {
        return QuaternionG<T>(-x, -y, -z, w);
    }
    QuaternionG<T> Normalized() const
    {
        T length = Length();
        if (length == 0.0f) { return QuaternionG<T>::Identity; }
        return QuaternionG<T>(x/length, y/length, z/length, w/length);
    }
    QuaternionG<T> Inversed() const
    {
        return Conjugated()/QuaternionG<T>::Dot(*this, *this);
    }

    Vector3G<T> GetEulerAngles() const
    {
        return QuaternionG<T>::GetEulerAngles(*this);
    }

    T GetPitch() const
    {
        return Cast<T>(
                    Math::ATan2(
                        (Cast<T>(2) * (y * z + w * x)),
                        (w * w - x * x - y * y + z * z)
                        )
                    );
    }

    T GetYaw() const
    {
        return Math::ASin(
                    Math::Clamp(
                        Cast<T>(-2) * (x * z - w * y),
                        Cast<T>(-1),
                        Cast<T>(1)
                        )
                    );
    }

    T GetRoll() const
    {
        return Cast<T>(
                    Math::ATan2(
                        (Cast<T>(2) * (x * y + w * z)),
                        (w * w + x * x - y * y - z * z)
                        )
                    );
    }

    static T Dot(const QuaternionG<T> &q1, const QuaternionG<T> &q2)
    {
        return q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
    }
    static QuaternionG<T> Cross(const QuaternionG<T> &q1,
                                const QuaternionG<T> &q2)
    {
        return QuaternionG<T>(q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
                              q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
                              q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x,
                              q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
    }

    Vector3G<T> GetAngleAxis() const
    {
        float angle = 2 * Math::ACos(w);

        float w2 = w*w;
        float sqrt = Math::Sqrt(1.0f - w2);

        Vector3 axis;
        if (sqrt != 0) { axis = Vector3(x / sqrt, y / sqrt, z / sqrt); }
        else { axis = Vector3::Zero; }

        return axis.NormalizedSafe() * angle;
    }
    static QuaternionG<T> FromEulerAngles(const Vector3 &eulerAnglesRads)
    {
        QuaternionG<T> qx = Quaternion::AngleAxis(eulerAnglesRads.x, Vector3::Right);
        QuaternionG<T> qy = Quaternion::AngleAxis(eulerAnglesRads.y, Vector3::Up);
        QuaternionG<T> qz = Quaternion::AngleAxis(eulerAnglesRads.z, Vector3::Forward);
        return (qz * qy * qx).Normalized();
    }
    static Vector3G<T> GetEulerAngles(const QuaternionG<T> &q)
    {
        return Vector3G<T>(q.GetPitch(), q.GetYaw(), q.GetRoll()).ToDegrees();
    }

    template<class Real = T>
    static QuaternionG<T> Lerp(const QuaternionG<T> &from,
                               const QuaternionG<T> &to,
                               Real t)
    {
        T cosTheta = QuaternionG<T>::Dot(from, to);

        if(cosTheta > Cast<T>(1) - Cast<T>(0.01))
        {
            return QuaternionG(Math::Lerp(from.x, to.x, t),
                               Math::Lerp(from.y, to.y, t),
                               Math::Lerp(from.z, to.z, t),
                               Math::Lerp(from.w, to.w, t));
        }
        else
        {
            T angle = Math::ACos(cosTheta);
            return (
                    Math::Sin((Cast<T>(1) - t) * angle) * from +
                    Math::Sin(t * angle) * to
                   ) /
                   Math::Sin(angle);
        }
    }

    template<class Real = T>
    static QuaternionG<T> SLerp(const QuaternionG<T> &from,
                                const QuaternionG<T> &_to,
                                Real t)
    {
        QuaternionG<T> to = _to;

        T cosTheta = QuaternionG<T>::Dot(from, _to);

        // If cosTheta < 0, the interpolation will take the long way around the sphere.
        // To fix this, one quat must be negated.
        if (cosTheta < Cast<T>(0))
        {
            to       = -_to;
            cosTheta = -cosTheta;
        }

        if(cosTheta > Cast<T>(1) - Cast<T>(0.01))
        {
            return QuaternionG<T>( Math::Lerp(from.x, to.x, t),
                                   Math::Lerp(from.y, to.y, t),
                                   Math::Lerp(from.z, to.z, t),
                                   Math::Lerp(from.w, to.w, t));
        }
        else
        {
            T angle = Math::ACos(cosTheta);
            return (
                        Math::Sin((Cast<T>(1) - t) * angle) * from +
                        Math::Sin(t * angle) * to
                        ) /
                    Math::Sin(angle);
        }
    }
    static QuaternionG<T> FromTo(const Vector3G<T> &from,
                                 const Vector3G<T> &to)
    {
        Vector3G<T> v0 = from.Normalized();
        Vector3G<T> v1 = to.Normalized();

        const T d = Vector3G<T>::Dot(v0, v1);
        if (d >= 1.0f) { return QuaternionG<T>::Identity; }
        else if (d <= -1.0f)
        {
            Vector3G<T> axis(1,0,0);
            axis = Vector3G<T>::Cross(axis, v0);
            if (axis.Length() == 0)
            {
                axis = Vector3G<T>(0, 1, 0);
                axis = Vector3G<T>::Cross(axis, v0);
            }

            return QuaternionG<T>(axis.x, axis.y, axis.z, 0.0f).Normalized();
        }

        const float s = float(Math::Sqrt( (1+d)*2 ));
        const float invs = 1.0f/s;
        const Vector3G<T> c = Vector3G<T>::Cross(v0, v1) * invs;
        return QuaternionG<T>(c.x, c.y, c.z, s * 0.5f).Normalized();
    }

    static QuaternionG<T> LookDirection(
                                    const Vector3G<T> &_forward,
                                    const Vector3G<T> &_up = Vector3G<T>::Up)
    {
        Vector3G<T> forward = _forward.Normalized();
        Vector3G<T> up      = _up.Normalized();

        if (Vector3G<T>::Dot(forward,  up) >= Cast<T>(0.99) ||
            Vector3G<T>::Dot(forward, -up) >= Cast<T>(0.99))
        {
            return QuaternionG<T>::Identity;
        }
        return Matrix4G<T>::ToQuaternion(
                    Matrix4G<T>::LookAt(Vector3::Zero, forward, up).Inversed());
    }

    template<class OtherT>
    static QuaternionG<T> AngleAxis(T angleRads, const Vector3G<OtherT> &axis)
    {
        const T s = Math::Sin(angleRads * Cast<T>(0.5));
        return QuaternionG<T>(Cast<T>(axis.x) * s,
                              Cast<T>(axis.y) * s,
                              Cast<T>(axis.z) * s,
                              Math::Cos(angleRads * Cast<T>(0.5))).Normalized();
    }

    static QuaternionG Identity;
};

template<class T>
QuaternionG<T> QuaternionG<T>::Identity = QuaternionG<T>();

template<class T>
bool operator==(const QuaternionG<T> &q1, const QuaternionG<T> &q2)
{
    return (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z) && (q1.w == q2.w);
}

template<class T>
bool operator!=(const QuaternionG<T> &q1, const QuaternionG<T> &q2)
{
    return !(q1 == q2);
}

template<class T>
QuaternionG<T> operator+(const QuaternionG<T> &q1, const QuaternionG<T> &q2)
{
    QuaternionG<T> res = q1;
    res += q2;
    return res;
}

template<class T>
QuaternionG<T>& operator+=(QuaternionG<T>& lhs, const QuaternionG<T> &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

template<class T, class OtherT>
QuaternionG<T> operator*(const QuaternionG<T> &q, OtherT a)
{
    return q * Cast<T>(a);
}

template<class T,class OtherT>
QuaternionG<T> operator*(OtherT a, const QuaternionG<T> &q)
{
    return QuaternionG<T>(q.x * Cast<T>(a),
                          q.y * Cast<T>(a),
                          q.z * Cast<T>(a),
                          q.w * Cast<T>(a));
}

template<class T, class OtherT>
QuaternionG<T> operator/(OtherT a, const QuaternionG<T> &q)
{
    return QuaternionG<T>(Cast<T>(a) / q.x,
                          Cast<T>(a) / q.y,
                          Cast<T>(a) / q.z,
                          Cast<T>(a) / q.w);
}

template<class T, class OtherT>
QuaternionG<T> operator/(const QuaternionG<T> &q, OtherT a)
{
    return QuaternionG<T>(q.x/a, q.y/a, q.z/a, q.w/a);
}
template<class T>
QuaternionG<T>& operator*=(QuaternionG<T> &lhs, const QuaternionG<T> &rhs)
{
    const QuaternionG<T> p(lhs);
    const QuaternionG<T> q(rhs);

    lhs.x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
    lhs.y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
    lhs.z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
    lhs.w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
    return lhs;
}
template<class T>
QuaternionG<T> operator*(const QuaternionG<T> &q1, const QuaternionG<T>& q2)
{
    QuaternionG<T> res = q1;
    res *= q2;
    return res;
}
template<class T>
QuaternionG<T> operator-(const QuaternionG<T> &q)
{
    return QuaternionG<T>(-q.x, -q.y, -q.z, -q.w);
}

template<class T>
Vector4G<T> operator*(QuaternionG<T> q, const Vector4G<T> &rhs)
{
    return Vector4G<T>(q * Vector3G<T>(rhs.x, rhs.y, rhs.w), rhs.w);
}

template<class T>
Vector4G<T> operator*(const Vector4G<T> &lhs, QuaternionG<T> q)
{
    return q.Inversed() * lhs;
}

template<class T>
Vector3G<T> operator*(const QuaternionG<T>& q, const Vector3G<T>& rhs)
{
    const Vector3G<T> qVector(q.x, q.y, q.z);
    const Vector3G<T> uv (Vector3G<T>::Cross(qVector, rhs));
    const Vector3G<T> uuv(Vector3G<T>::Cross(qVector, uv));

    return rhs + ((uv * q.w) + uuv) * Cast<T>(2);
}
template<class T>
Vector3G<T> operator*(const Vector3G<T>& lhs, const QuaternionG<T>& q)
{
    return q.Inversed() * lhs;
}

NAMESPACE_BANG_END

#endif // QUATERNIONG_H
