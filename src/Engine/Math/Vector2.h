#ifndef VECTOR2_H
#define VECTOR2_H

#include "Bang/Math.h"

template<class T>
class Vector2G
{
public:
    T x, y;

    Vector2G() : x(SCAST<T>(0)), y(SCAST<T>(0))
    {
    }

    explicit Vector2G(const T& a) : x(a), y(a)
    {
    }

    explicit Vector2G(const T& _x, const T& _y) : x(_x), y(_y)
    {
    }

    T Length() const
    {
        return Math::Sqrt(x*x + y*y);
    }

    void Normalize()
    {
        float l = Length();
        x /= l;
        y /= l;
    }

    Vector2G NormalizedSafe() const
    {
        Vector2G<T> v(*this);
        if (v == Vector2G<T>::Zero) { return Vector2G<T>::Zero; }
        v.Normalize();
        return v;
    }

    Vector2G Normalized() const
    {
        Vector2G<T> v(*this);
        v.Normalize();
        return v;
    }

    Vector2G ToDegrees() const
    {
        return Vector2G<T>( Math::Rad2Deg(x), Math::Rad2Deg(y) );
    }

    Vector2G ToRadians() const
    {
        return Vector2G<T>( Math::Deg2Rad(x), Math::Deg2Rad(y) );
    }

    T Distance(const Vector2G<T> &p) const
    {
        return Vector2G<T>::Distance(*this, p);
    }

    template<class OtherT1, class OtherT2, class Real>
    static Vector2G<T> Lerp(const Vector2G<OtherT1> &v1,
                            const Vector2G<OtherT2> &v2,
                            const Real& t)
    {
        return v1 + (v2 - v1) * t;
    }

    Vector2G<T> Abs() const
    {
        return Vector2G<T>(Math::Abs(x), Math::Abs(y));
    }

    T* Data() const
    {
        return SCAST<T*>(&x);
    }

    static Vector2G<T> Abs(const Vector2G<T> &v)
    {
        return v.Abs();
    }

    template<class OtherT>
    static T Dot(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
    {
        return (v1.x * SCAST<OtherT>(v2.x)) + (v1.y * SCAST<OtherT>(v2.y));
    }

    template<class OtherT>
    static T Distance(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
    {
        return (v1 - v2).Length();
    }

    template<class OtherT>
    static Vector2G<T> Max(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
    {
        return Vector2G<T>(Math::Max(v1.x, v2.x),
                           Math::Max(v1.y, v2.y));
    }

    template<class OtherT>
    static Vector2G<T> Min(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
    {
        return Vector2G<T>(Math::Min(v1.x, v2.x),
                           Math::Min(v1.y, v2.y));
    }

    template<class OtherT1, class OtherT2>
    static Vector2G<T> Clamp(const Vector2G<T> &v,
                             const Vector2G<OtherT1> &min,
                             const Vector2G<OtherT2> &max)
    {
        return Vector2G<T>::Min( Vector2G<T>::Max(v, max), min );
    }

    template<class OtherT>
    bool operator==(const Vector2G<OtherT> &lhs) const
    {
        return x == lhs.x && y == lhs.y;
    }
    template<class OtherT>
    bool operator!=(const Vector2G<OtherT> &lhs) const
    {
        return !(*this == lhs);
    }

    const static Vector2G Up;
    const static Vector2G Down;
    const static Vector2G Right;
    const static Vector2G Left;
    const static Vector2G Zero;
    const static Vector2G One;
};

template<class T>
const Vector2G<T> Vector2G<T>::Up = Vector2G<T>(SCAST<T>(0), SCAST<T>(1));
template<class T>
const Vector2G<T> Vector2G<T>::Down = Vector2G<T>(SCAST<T>(0), SCAST<T>(-1));
template<class T>
const Vector2G<T> Vector2G<T>::Right = Vector2G<T>(SCAST<T>(1), SCAST<T>(1));
template<class T>
const Vector2G<T> Vector2G<T>::Left = Vector2G<T>(SCAST<T>(-1), SCAST<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::Zero = Vector2G<T>(SCAST<T>(0), SCAST<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::One = Vector2G<T>(SCAST<T>(1), SCAST<T>(1));

template<class T, class OtherT>
Vector2G<T> operator+(const Vector2G<T> & v1, const Vector2G<OtherT> &v2)
{
    return Vector2G<T>(v1.x + SCAST<T>(v2.x), v1.y + SCAST<T>(v2.y));
}

template<class T, class OtherT>
Vector2G<T> operator*(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
{
    return Vector2G<T>(v1.x * SCAST<T>(v2.x), v1.y * SCAST<T>(v2.y));
}

template<class T, class OtherT>
Vector2G<T> operator-(const Vector2G<T> &v)
{
    return Vector2G<T>(-v.x, -v.y);
}

template<class T, class OtherT>
Vector2G<T> operator*(const OtherT &a, const Vector2G<T> &v)
{
    return Vector2G<T>(SCAST<T>(a) * v.x, SCAST<T>(a) * v.y);
}

template<class T, class OtherT>
Vector2G<T> operator*(const Vector2G<T> &v, const OtherT &a)
{
    return SCAST<T>(a) * v;
}

template<class T, class OtherT>
Vector2G<T> operator/(const OtherT &a, const Vector2G<T> &v)
{
    return Vector2G<T>(SCAST<T>(a) / v.x, SCAST<T>(a) / v.y);
}

template<class T, class OtherT>
Vector2G<T> operator/(const Vector2G<T> &v, const OtherT &a)
{
    return Vector2G<T>(v.x / SCAST<T>(a), v.y / SCAST<T>(a));
}

template<class T, class OtherT>
Vector2G<T> operator/(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
{
    return Vector2G<T>(v1.x / SCAST<T>(v2.x), v1.y / SCAST<T>(v2.y));
}

template<class T, class OtherT>
Vector2G<T> &operator+=(Vector2G<T> &lhs, const Vector2G<OtherT> &rhs)
{
    lhs.x += SCAST<T>(rhs.x);
    lhs.y += SCAST<T>(rhs.y);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator-=(Vector2G<T> &lhs, const Vector2G<OtherT> &rhs)
{
    lhs.x -= SCAST<T>(rhs.x);
    lhs.y -= SCAST<T>(rhs.y);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator*=(Vector2G<T> &lhs, const Vector2G<OtherT> &rhs)
{
    lhs.x *= SCAST<T>(rhs.x);
    lhs.y *= SCAST<T>(rhs.y);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator/=(Vector2G<T> &lhs, const Vector2G<OtherT> &rhs)
{
    lhs.x /= SCAST<T>(rhs.x);
    lhs.y /= SCAST<T>(rhs.y);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> operator+(const OtherT &a, const Vector2G<T> &v)
{
    return Vector2G<T>(SCAST<T>(a) + v.x, SCAST<T>(a) + v.y);
}

template<class T, class OtherT>
Vector2G<T> operator+(const Vector2G<T> &v, const OtherT &a)
{
    return SCAST<T>(a) + v;
}

template<class T, class OtherT>
Vector2G<T> operator-(const Vector2G<T> &v1, const Vector2G<OtherT> &v2)
{
    return Vector2G<T>(v1.x - SCAST<T>(v2.x), v1.y - SCAST<T>(v2.y));
}

template<class T, class OtherT>
Vector2G<T> operator-(const OtherT &a, const Vector2G<T> &v)
{
    return Vector2G<T>(SCAST<T>(a) - v.x, SCAST<T>(a) - v.y);
}

template<class T, class OtherT>
Vector2G<T> operator-(const Vector2G<T> &v, const OtherT &a)
{
    return Vector2G<T>(v.x - SCAST<T>(a), v.y - SCAST<T>(a));
}

template<class T, class OtherT>
Vector2G<T> &operator+=(Vector2G<T> &lhs, const OtherT &a)
{
    lhs.x += SCAST<T>(a);
    lhs.y += SCAST<T>(a);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator-=(Vector2G<T> &lhs, const T &a)
{
    lhs.x -= SCAST<T>(a);
    lhs.y -= SCAST<T>(a);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator*=(Vector2G<T> &lhs, const OtherT &a)
{
    lhs.x *= SCAST<T>(a);
    lhs.y *= SCAST<T>(a);
    return lhs;
}

template<class T, class OtherT>
Vector2G<T> &operator/=(Vector2G<T> &lhs, const OtherT &a)
{
    lhs.x /= SCAST<T>(a);
    lhs.y /= SCAST<T>(a);
    return lhs;
}

using Vector2f = Vector2G<float>;
using Vector2d = Vector2G<double>;
using Vector2i = Vector2G<int>;
using Vector2u = Vector2G<uint>;
using Vector2  = Vector2f;

#endif // VECTOR2_H
