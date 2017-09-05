#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <type_traits>

#include "Bang/Math.h"

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

#define CLASS_VECTOR_T(VECTOR_G, N) \
template<class T> \
class VECTOR_G \
{ \
public: \
    T XYZ_MEMBERS; \
 \
    VECTOR_G() \
    { \
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(0); } \
    } \
 \
    explicit VECTOR_G(T a) \
    { \
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(a); } \
    } \
 \
    template<class OtherT> \
    explicit VECTOR_G(const VECTOR_G<OtherT>& v) \
    { \
        for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(v[i]); } \
    } \
 \
    T Length() const \
    { \
        return Math::Sqrt(SqLength()); \
    } \
 \
    T SqLength() const \
    { \
        T res = SCAST<T>(0); \
        for (int i = 0; i < N; ++i) { res += At(i) * At(i); } \
        return res; \
    } \
    void Normalize() \
    { \
        *this /= Length(); \
    } \
 \
    VECTOR_G NormalizedSafe() const \
    { \
        if (*this == VECTOR_G<T>::Zero) { return VECTOR_G<T>::Zero; } \
        return (*this).Normalized(); \
    } \
 \
    VECTOR_G Normalized() const \
    { \
        VECTOR_G<T> v(*this); \
        v.Normalize(); \
        return v; \
    } \
 \
    VECTOR_G ToDegrees() const \
    { \
        VECTOR_G res(*this); \
        for (int i = 0; i < N; ++i) { res[i] = Math::Rad2Deg( res[i] ); } \
        return res; \
    } \
 \
    VECTOR_G ToRadians() const \
    { \
        VECTOR_G res(*this); \
        for (int i = 0; i < N; ++i) { res[i] = Math::Deg2Rad( res[i] ); } \
        return res; \
    } \
 \
    T Distance(const VECTOR_G<T> &p) const \
    { \
        return VECTOR_G<T>::Distance(*this, p); \
    } \
    \
    T SqDistance(const VECTOR_G<T> &p) const \
    { \
        return VECTOR_G<T>::SqDistance(*this, p); \
    } \
 \
    \
    T &At(std::size_t i) \
    { \
        return (*this)[i]; \
    } \
    const T &At(std::size_t i) const \
    { \
        return (*this)[i]; \
    } \
 \
    template<class Real> \
    static VECTOR_G<T> Lerp(const VECTOR_G<T> &v1, \
                            const VECTOR_G<T> &v2, \
                            Real t) \
    { \
        return v1 + (v2 - v1) * t; \
    } \
 \
    VECTOR_G<T> Abs() const \
    { \
        VECTOR_G res(*this); \
        for (int i = 0; i < N; ++i) { res[i] = Math::Abs( res[i] ); } \
        return res; \
    } \
 \
    const T* Data() const { return &At(0); } \
    T* Data() { return &At(0); } \
 \
    static VECTOR_G<T> Abs(const VECTOR_G<T> &v) \
    { \
        return v.Abs(); \
    } \
 \
    static T Dot(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
    { \
        T res = SCAST<T>(0); \
        for (int i = 0; i < N; ++i) { res += v1[i] * v2[i]; } \
        return res; \
    } \
 \
    static T Distance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
    { \
        return (v1 - v2).Length(); \
    } \
 \
    static T SqDistance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
    { \
        return (v1 - v2).SqLength(); \
    } \
 \
    static VECTOR_G<T> Max(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Max(v1[i], v2[i]); } \
        return res; \
    } \
 \
    static VECTOR_G<T> Min(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Min(v1[i], v2[i]); } \
        return res; \
    } \
 \
    static VECTOR_G<T> Floor(const VECTOR_G<T> &v1) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Floor(v1[i]); } \
        return res; \
    } \
 \
    static VECTOR_G<T> Ceil(const VECTOR_G<T> &v1) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Ceil(v1[i]); } \
        return res; \
    } \
 \
    static VECTOR_G<T> Round(const VECTOR_G<T> &v1) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Round(v1[i]); } \
        return res; \
    } \
 \
    static VECTOR_G<T> Clamp(const VECTOR_G<T> &v, \
                             const VECTOR_G<T> &min, \
                             const VECTOR_G<T> &max) \
    { \
        return VECTOR_G<T>::Min( VECTOR_G<T>::Max(v, max), min ); \
    } \
 \
    const T& operator[](std::size_t i) const \
    { \
        return (reinterpret_cast<const T*>(this))[i]; \
    } \
    T& operator[](std::size_t i) \
    { \
        return (reinterpret_cast<T*>(this))[i]; \
    } \
    bool operator==(const VECTOR_G<T> &rhs) const \
    { \
        for (int i = 0; i < N; ++i) { if (At(i) != rhs[i]) { return false; } } \
        return true; \
    } \
    bool operator!=(const VECTOR_G<T> &rhs) const \
    { \
        return !(*this == rhs); \
    } \
 \
    EXTRA_DECLARATIONS \
}; \
\
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> & v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] + v2[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] * v2[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator*(T a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a * v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v, T a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] * a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator/(T a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a / v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v, T a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] / a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] / v2[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] += rhs[i]; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] -= rhs[i]; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] *= rhs[i]; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] /= rhs[i]; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> operator+(T a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a + v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> &v, T a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] + a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] - v2[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator-(T a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a - v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v, T a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] - a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, T a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] += a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, T a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] -= a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, T a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] *= a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, T a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] /= a; } \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v) \
{ \
    return v * SCAST<T>(-1); \
}


#endif // VECTOR_H
