#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
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
    explicit VECTOR_G(const T& a) \
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
    template<class OtherT1, class OtherT2, class Real> \
    static VECTOR_G<T> Lerp(const VECTOR_G<OtherT1> &v1, \
                            const VECTOR_G<OtherT2> &v2, \
                            const Real& t) \
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
    template<class OtherT> \
    static T Dot(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
    { \
        T res = SCAST<T>(0); \
        for (int i = 0; i < N; ++i) { res += v1[i] * SCAST<T>(v2[i]); } \
        return res; \
    } \
 \
    template<class OtherT> \
    static T Distance(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
    { \
        return (v1 - v2).Length(); \
    } \
 \
    template<class OtherT> \
    static T SqDistance(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
    { \
        return (v1 - v2).SqLength(); \
    } \
 \
    template<class OtherT> \
    static VECTOR_G<T> Max(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Max(v1[i], SCAST<T>(v2[i])); } \
        return res; \
    } \
 \
    template<class OtherT> \
    static VECTOR_G<T> Min(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
    { \
        VECTOR_G res; \
        for (int i = 0; i < N; ++i) { res[i] = Math::Min(v1[i], SCAST<T>(v2[i])); } \
        return res; \
    } \
 \
    template<class OtherT1, class OtherT2> \
    static VECTOR_G<T> Clamp(const VECTOR_G<T> &v, \
                             const VECTOR_G<OtherT1> &min, \
                             const VECTOR_G<OtherT2> &max) \
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
    template<class OtherT> \
    bool operator==(const VECTOR_G<OtherT> &rhs) const \
    { \
        for (int i = 0; i < N; ++i) { if (At(i) != rhs[i]) { return false; } } \
        return true; \
    } \
    template<class OtherT> \
    bool operator!=(const VECTOR_G<OtherT> &rhs) const \
    { \
        return !(*this == rhs); \
    } \
 \
    EXTRA_DECLARATIONS \
}; \
\
\
template<class T, class OtherT> \
VECTOR_G<T> operator+(const VECTOR_G<T> & v1, const VECTOR_G<OtherT> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] + SCAST<T>(v2[i]); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] * SCAST<T>(v2[i]); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator*(const OtherT &a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = SCAST<T>(a) * v[i]; }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v, const OtherT &a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] * SCAST<T>(a); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator/(const OtherT &a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = SCAST<T>(a) / v[i]; }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v, const OtherT &a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] / SCAST<T>(a); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] / SCAST<T>(v2[i]); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, const VECTOR_G<OtherT> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] += SCAST<T>(rhs[i]); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, const VECTOR_G<OtherT> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] -= SCAST<T>(rhs[i]); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, const VECTOR_G<OtherT> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] *= SCAST<T>(rhs[i]); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, const VECTOR_G<OtherT> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] /= SCAST<T>(rhs[i]); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator+(const OtherT &a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = SCAST<T>(a) + v[i]; }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator+(const VECTOR_G<T> &v, const OtherT &a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] + SCAST<T>(a); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v1, const VECTOR_G<OtherT> &v2) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v1[i] - SCAST<T>(v2[i]); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator-(const OtherT &a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = SCAST<T>(a) - v[i]; }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v, const OtherT &a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] - SCAST<T>(a); }  \
    return res; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, const OtherT &a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] += SCAST<T>(a); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, const OtherT &a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] -= SCAST<T>(a); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, const OtherT &a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] *= SCAST<T>(a); }  \
    return lhs; \
} \
\
template<class T, class OtherT> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, const OtherT &a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] /= SCAST<T>(a); } \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v) \
{ \
    return v * -1; \
}


#endif // VECTOR_H
