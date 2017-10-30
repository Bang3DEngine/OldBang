#ifndef QUATERNIONG_H
#define QUATERNIONG_H

#include <iostream>

#include "Bang/Debug.h"

NAMESPACE_BANG_BEGIN

template<class T> // Most of it almost copied from glm
class QuaternionG
{
public:
    T x, y, z, w;

    QuaternionG() : x( SCAST<T>(0) ),
                    y( SCAST<T>(0) ),
                    z( SCAST<T>(0) ),
                    w( SCAST<T>(1) )
    {
    }

    QuaternionG(T _x, T _y, T _z, T _w) :
                    x( SCAST<T>(_x) ),
                    y( SCAST<T>(_y) ),
                    z( SCAST<T>(_z) ),
                    w( SCAST<T>(_w) )
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
        return QuaternionG<T>(x/length, y/length, z/length, w/length);
    }
    QuaternionG<T> Inversed() const
    {
        return Conjugated()/QuaternionG<T>::Dot(*this, *this);
    }

    Vector3G<T> EulerAngles() const
    {
        return QuaternionG<T>::EulerAngles(*this);
    }

    T Pitch() const
    {
        return SCAST<T>(
                    Math::ATan2(
                        (SCAST<T>(2) * (y * z + w * x)),
                        (w * w - x * x - y * y + z * z)
                        )
                    );
    }

    T Yaw() const
    {
        return Math::ASin(
                    Math::Clamp(
                        SCAST<T>(-2) * (x * z - w * y),
                        SCAST<T>(-1),
                        SCAST<T>(1)
                        )
                    );
    }

    T Roll() const
    {
        return SCAST<T>(
                    Math::ATan2(
                        (SCAST<T>(2) * (x * y + w * z)),
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

    static Vector3G<T> EulerAngles(const QuaternionG<T> &q)
    {
        return Vector3G<T>(q.Pitch(), q.Yaw(), q.Roll());
    }

    template<class Real>
    static QuaternionG<T> Lerp(const QuaternionG<T> &from,
                               const QuaternionG<T> &to,
                               Real t)
    {
        T cosTheta = QuaternionG<T>::Dot(from, to);

        if(cosTheta > SCAST<T>(1) - SCAST<T>(0.01))
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
                        Math::Sin((SCAST<T>(1) - t) * angle) * from +
                        Math::Sin(t * angle) * to
                        ) /
                    Math::Sin(angle);
        }
    }

    template<class Real>
    static QuaternionG<T> SLerp(const QuaternionG<T> &from,
                                const QuaternionG<T> &_to,
                                float t)
    {
        QuaternionG<T> to = _to;

        T cosTheta = QuaternionG<T>::Dot(from, _to);

        // If cosTheta < 0, the interpolation will take the long way around the sphere.
        // To fix this, one quat must be negated.
        if (cosTheta < SCAST<T>(0))
        {
            to       = -_to;
            cosTheta = -cosTheta;
        }

        if(cosTheta > SCAST<T>(1) - SCAST<T>(0.01))
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
                        Math::Sin((SCAST<T>(1) - t) * angle) * from +
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

            return QuaternionG<T>(0.0f, axis.x, axis.y, axis.z).Normalized();
        }

        const float s = float(Math::Sqrt( (1+d)*2 ));
        const float invs = 1.0f/s;
        const Vector3G<T> c = Vector3G<T>::Cross(v0, v1) * invs;
        return QuaternionG<T>(s * 0.5f, c.x, c.y, c.z).Normalized();
    }

    static QuaternionG<T> LookDirection(
                                    const Vector3G<T> &_forward,
                                    const Vector3G<T> &_up = Vector3G<T>::Up)
    {
        Vector3G<T> forward = _forward.Normalized();
        Vector3G<T> up      = _up.Normalized();

        if (Vector3G<T>::Dot(forward,  up) >= SCAST<T>(0.99) ||
            Vector3G<T>::Dot(forward, -up) >= SCAST<T>(0.99))
        {
            Debug_Warn("LookDirection: Forward and up aligned."
                         " Returning identity");
            return QuaternionG<T>::Identity;
        }
        return Matrix4G<T>::ToQuaternion(
                    Matrix4G<T>::LookAt(Vector3::Zero, forward, up).Inversed());
    }

    template<class OtherT>
    static QuaternionG<T> AngleAxis(T angleRads, const Vector3G<OtherT> &axis)
    {
        const T s = Math::Sin(angleRads * SCAST<T>(0.5));
        return QuaternionG<T>(SCAST<T>(axis.x) * s,
                              SCAST<T>(axis.y) * s,
                              SCAST<T>(axis.z) * s,
                              Math::Cos(angleRads * SCAST<T>(0.5)));
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
    return q * SCAST<T>(a);
}

template<class T,class OtherT>
QuaternionG<T> operator*(OtherT a, const QuaternionG<T> &q)
{
    return QuaternionG<T>(q.x * SCAST<T>(a),
                          q.y * SCAST<T>(a),
                          q.z * SCAST<T>(a),
                          q.w * SCAST<T>(a));
}

template<class T, class OtherT>
QuaternionG<T> operator/(OtherT a, const QuaternionG<T> &q)
{
    return QuaternionG<T>(SCAST<T>(a) / q.x,
                          SCAST<T>(a) / q.y,
                          SCAST<T>(a) / q.z,
                          SCAST<T>(a) / q.w);
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

    return rhs + ((uv * q.w) + uuv) * SCAST<T>(2);
}
template<class T>
Vector3G<T> operator*(const Vector3G<T>& lhs, const QuaternionG<T>& q)
{
    return q.Inversed() * lhs;
}

NAMESPACE_BANG_END

#endif // QUATERNIONG_H
