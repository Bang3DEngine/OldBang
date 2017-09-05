#ifndef VECTORG_H
#define VECTORG_H

#include <cstdlib>
#include <type_traits>

#include "Bang/Math.h"

template<uint N, class T>
class VectorGCommon
{
public:
    VectorGCommon()
    {
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(0); }
    }

    explicit VectorGCommon(const T& a)
    {
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(a); }
    }

    template<class OtherT>
    explicit VectorGCommon(const VectorGCommon<N, OtherT>& v)
    {
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(v[i]); }
    }

    T Length() const
    {
        return Math::Sqrt(SqLength());
    }

    T SqLength() const
    {
        T res = SCAST<T>(0);
        for (int i = 0; i < N; ++i) { res += At(i) * At(i); }
        return res;
    }

    void Normalize()
    {
        float len = Length();
        for (int i = 0; i < N; ++i) { At(i) = At(i) / len; }
    }

    VectorGCommon NormalizedSafe() const
    {
        if (*this == VectorGCommon<N,T>::Zero) { return VectorGCommon<N,T>::Zero; }
        return (*this).Normalized();
    }

    VectorGCommon Normalized() const
    {
        VectorGCommon<N,T> v(*this);
        v.Normalize();
        return v;
    }

    VectorGCommon ToDegrees() const
    {
        VectorGCommon res(*this);
        for (int i = 0; i < N; ++i) { res[i] = Math::Rad2Deg( res[i] ); }
        return res;
    }

    VectorGCommon ToRadians() const
    {
        VectorGCommon res(*this);
        for (int i = 0; i < N; ++i) { res[i] = Math::Deg2Rad( res[i] ); }
        return res;
    }

    T &At(std::size_t i)
    {
        return (*this)[i];
    }
    const T &At(std::size_t i) const
    {
        return (*this)[i];
    }

    VectorGCommon<N,T> Abs() const
    {
        VectorGCommon<N,T> res(*this);
        for (int i = 0; i < N; ++i) { res[i] = Math::Abs( res[i] ); }
        return res;
    }

    const T* Data() const { return &At(0); }
    T* Data() { return &At(0); }

    const T& operator[](std::size_t i) const
    {
        return (RCAST<const T*>(this))[i];
    }
    T& operator[](std::size_t i)
    {
        return (RCAST<T*>(this))[i];
    }
    bool operator==(const VectorGCommon<N,T> &rhs) const
    {
        for (int i = 0; i < N; ++i) { if (At(i) != rhs[i]) { return false; } }
        return true;
    }
    bool operator!=(const VectorGCommon<N,T> &rhs) const
    {
        return !(*this == rhs);
    }
};

template<uint N, class T>
VectorG<N,T> operator+(const VectorG<N,T> & v1, const VectorG<N,T> &v2)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v1[i] + v2[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator*(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v1[i] * v2[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator*(T a, const VectorG<N,T> &v)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = a * v[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator*(const VectorG<N,T> &v, T a)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v[i] * a; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator/(T a, const VectorG<N,T> &v)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = a / v[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator/(const VectorG<N,T> &v, T a)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v[i] / a; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator/(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v1[i] / v2[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> &operator+=(VectorG<N,T> &lhs, const VectorG<N,T> &rhs)
{
    for (int i = 0; i < N; ++i) { lhs[i] += rhs[i]; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator-=(VectorG<N,T> &lhs, const VectorG<N,T> &rhs)
{
    for (int i = 0; i < N; ++i) { lhs[i] -= rhs[i]; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator*=(VectorG<N,T> &lhs, const VectorG<N,T> &rhs)
{
    for (int i = 0; i < N; ++i) { lhs[i] *= rhs[i]; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator/=(VectorG<N,T> &lhs, const VectorG<N,T> &rhs)
{
    for (int i = 0; i < N; ++i) { lhs[i] /= rhs[i]; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> operator+(const T& a, const VectorG<N,T> &v)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = a + v[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator+(const VectorG<N,T> &v, const T& a)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v[i] + a; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator-(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v1[i] - v2[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator-(const T& a, const VectorG<N,T> &v)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = a - v[i]; }
    return res;
}

template<uint N, class T>
VectorG<N,T> operator-(const VectorG<N,T> &v, const T& a)
{
    VectorG<N,T> res;
    for (int i = 0; i < N; ++i) { res[i] = v[i] - a; }
    return res;
}

template<uint N, class T>
VectorG<N,T> &operator+=(VectorG<N,T> &lhs, const T& a)
{
    for (int i = 0; i < N; ++i) { lhs[i] += a; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator-=(VectorG<N,T> &lhs, const T& a)
{
    for (int i = 0; i < N; ++i) { lhs[i] -= a; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator*=(VectorG<N,T> &lhs, const T& a)
{
    for (int i = 0; i < N; ++i) { lhs[i] *= a; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> &operator/=(VectorG<N,T> &lhs, const T& a)
{
    for (int i = 0; i < N; ++i) { lhs[i] /= a; }
    return lhs;
}

template<uint N, class T>
VectorG<N,T> operator-(const VectorG<N,T> &v)
{
    return v * SCAST<T>(-1);
}

template<uint N, class T>
class VectorG : public VectorGCommon<N,T>
{
    using VectorGCommon<N,T>::VectorGCommon;
    VectorG(const VectorGCommon<N,T> &v) {}
};

#endif // VECTORG_H
