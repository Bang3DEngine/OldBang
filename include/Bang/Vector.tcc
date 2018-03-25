#ifndef VECTOR_TCC
#define VECTOR_TCC

#include "Bang/Math.h"
#include "Bang/Axis.h"
#include "Bang/TypeTraits.h"

#define CLASS_VECTOR_T_IMPL(VECTOR_G, N) \
\
template<class T> \
VECTOR_G<T>::VECTOR_G() \
{ \
    for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(0); } \
} \
\
template<class T> \
VECTOR_G<T>::VECTOR_G(const T& a) \
{ \
    for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(a); } \
} \
 \
template<class T> \
template<class OtherT> \
VECTOR_G<T>::VECTOR_G(const VECTOR_G<OtherT>& v) \
{ \
    for (int i = 0; i < N; ++i) { At(i) = SCAST<T>(v[i]); } \
} \
\
template<class T> \
T VECTOR_G<T>::Length() const \
{ \
    return Math::Sqrt(SqLength()); \
} \
\
template<class T> \
T VECTOR_G<T>::SqLength() const \
{ \
    T res = SCAST<T>(0); \
    for (int i = 0; i < N; ++i) { res += At(i) * At(i); } \
    return res; \
} \
\
template<class T> \
void VECTOR_G<T>::Normalize() \
{ \
    *this /= Length(); \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::NormalizedSafe() const \
{ \
    if (*this == VECTOR_G<T>::Zero) { return VECTOR_G<T>::Zero; } \
    return (*this).Normalized(); \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Normalized() const \
{ \
    VECTOR_G<T> v(*this); \
    v.Normalize(); \
    return v; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::ToDegrees() const \
{ \
    VECTOR_G<T> res(*this); \
    for (int i = 0; i < N; ++i) { res[i] = Math::RadToDeg( res[i] ); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::ToRadians() const \
{ \
    VECTOR_G<T> res(*this); \
    for (int i = 0; i < N; ++i) { res[i] = Math::DegToRad( res[i] ); } \
    return res; \
} \
\
template<class T> \
T VECTOR_G<T>::Distance(const VECTOR_G<T> &p) const \
{ \
    return VECTOR_G<T>::Distance(*this, p); \
} \
\
template<class T> \
T VECTOR_G<T>::SqDistance(const VECTOR_G<T> &p) const \
{ \
    return VECTOR_G<T>::SqDistance(*this, p); \
} \
\
template<class T> \
T& VECTOR_G<T>::At(std::size_t i) \
{ \
    return (*this)[i]; \
} \
template<class T> \
const T& VECTOR_G<T>::At(std::size_t i) const \
{ \
    return (*this)[i]; \
} \
\
template<class T> \
template<class Real> \
VECTOR_G<T> VECTOR_G<T>::Lerp(const VECTOR_G<T> &v1, \
                              const VECTOR_G<T> &v2, \
                              Real t) \
{ \
    return v1 + (v2 - v1) * t; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Abs() const \
{ \
    VECTOR_G res(*this); \
    for (int i = 0; i < N; ++i) { res[i] = Math::Abs( res[i] ); } \
    return res; \
} \
\
template<class T> \
T* VECTOR_G<T>::Data() { return &At(0); } \
template<class T> \
const T* VECTOR_G<T>::Data() const { return &At(0); } \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Abs(const VECTOR_G<T> &v) \
{ \
    return v.Abs(); \
} \
\
template<class T> \
T VECTOR_G<T>::Dot(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    T res = SCAST<T>(0); \
    for (int i = 0; i < N; ++i) { res += v1[i] * v2[i]; } \
    return res; \
} \
\
template<class T> \
T VECTOR_G<T>::Distance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    return (v1 - v2).Length(); \
} \
\
template<class T> \
T VECTOR_G<T>::SqDistance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    return (v1 - v2).SqLength(); \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Max(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G res; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Max(v1[i], v2[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Min(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
{ \
    VECTOR_G res; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Min(v1[i], v2[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Floor(const VECTOR_G<T> &v1) \
{ \
    VECTOR_G res; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Floor(v1[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Ceil(const VECTOR_G<T> &v1) \
{ \
    VECTOR_G res; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Ceil(v1[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Round(const VECTOR_G<T> &v1) \
{ \
    VECTOR_G res; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Round(v1[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Clamp(const VECTOR_G<T> &v, \
                               const VECTOR_G<T> &min, \
                               const VECTOR_G<T> &max) \
{ \
    VECTOR_G res = v; \
    for (int i = 0; i < N; ++i) { res[i] = Math::Clamp(res[i], min[i], max[i]); } \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> VECTOR_G<T>::Clamp2(const VECTOR_G<T> &v, \
                                const VECTOR_G<T> &bound1, \
                                const VECTOR_G<T> &bound2) \
{ \
    VECTOR_G res = v; \
    for (int i = 0; i < N; ++i) { \
        res[i] = Math::Clamp(res[i], \
                             Math::Min(bound1[i], bound2[i]), \
                             Math::Max(bound1[i], bound2[i])); \
    } \
    return res; \
} \
\
template<class T> \
Axis VECTOR_G<T>::GetAxis() const \
{ \
    return x == 1 ? Axis::Horizontal : Axis::Vertical; \
} \
template<class T> \
const T& VECTOR_G<T>::GetAxis(Axis axis) const \
{ \
    return (axis == Axis::Horizontal) ? x : y; \
} \
template<class T> \
VECTOR_G<T> VECTOR_G<T>::FromAxis(Axis axis) \
{ \
    return (axis == Axis::Horizontal) ? VECTOR_G<T>::Right : VECTOR_G<T>::Up; \
} \
\
template<class T> \
const T& VECTOR_G<T>::operator[](std::size_t i) const \
{ \
    return (RCAST<const T*>(this))[i]; \
} \
template<class T> \
T& VECTOR_G<T>::operator[](std::size_t i) \
{ \
    return (RCAST<T*>(this))[i]; \
} \
\
/* Operators */ \
\
template<class T> \
bool operator==(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { if (lhs[i] != rhs[i]) { return false; } } \
    return true; \
} \
\
template<class T> \
bool operator<(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { if (lhs[i] >= rhs[i]) { return false; } } \
    return true; \
} \
\
template<class T> \
bool operator<=(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    for (int i = 0; i < N; ++i) { if (lhs[i] > rhs[i]) { return false; } } \
    return true; \
} \
\
template<class T> \
bool operator>(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    return (rhs < lhs); \
} \
\
template<class T> \
bool operator>=(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    return (rhs <= lhs); \
} \
\
template<class T> \
bool operator!=(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs) \
{ \
    return !(lhs == rhs); \
} \
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2) \
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
VECTOR_G<T> operator*(const T& a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a * v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v, const T& a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] * a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator/(const T& a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a / v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v, const T& a) \
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
VECTOR_G<T> operator+(const T& a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a + v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> &v, const T& a) \
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
VECTOR_G<T> operator-(const T& a, const VECTOR_G<T> &v) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = a - v[i]; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v, const T& a) \
{ \
    VECTOR_G<T> res; \
    for (int i = 0; i < N; ++i) { res[i] = v[i] - a; }  \
    return res; \
} \
\
template<class T> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, const T& a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] += a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, const T& a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] -= a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, const T& a) \
{ \
    for (int i = 0; i < N; ++i) { lhs[i] *= a; }  \
    return lhs; \
} \
\
template<class T> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, const T& a) \
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
