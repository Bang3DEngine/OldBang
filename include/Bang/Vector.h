#ifndef VECTOR_H
#define VECTOR_H

#include "Bang/Math.h"
#include "Bang/Axis.h"
#include "Bang/TypeTraits.h"

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

#define CLASS_VECTOR_T(VECTOR_G, N) \
\
template<class T> \
class VECTOR_G \
{ \
public: \
    T XYZ_MEMBERS; \
    \
    VECTOR_G(); \
    explicit VECTOR_G(const T& a); \
    \
    template<class OtherT> \
    explicit VECTOR_G(const VECTOR_G<OtherT>& v); \
    \
    T Length() const;   \
    T SqLength() const; \
    void Normalize();   \
    \
    VECTOR_G NormalizedSafe() const; \
    VECTOR_G Normalized() const;     \
    \
    VECTOR_G ToDegrees() const; \
    VECTOR_G ToRadians() const; \
    \
    T Distance(const VECTOR_G<T> &p) const; \
    T SqDistance(const VECTOR_G<T> &p) const; \
    \
    VECTOR_G<T> Abs() const; \
    \
    T &At(std::size_t i); \
    const T &At(std::size_t i) const; \
    \
    T* Data(); \
    const T* Data() const; \
    \
    static T Dot(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
    \
    static T Distance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
    static T SqDistance(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
    \
    static VECTOR_G<T> Abs(const VECTOR_G<T> &v); \
    static VECTOR_G<T> Max(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
    static VECTOR_G<T> Min(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
    \
    static VECTOR_G<T> Floor(const VECTOR_G<T> &v1);      \
    static VECTOR_G<T> Ceil(const VECTOR_G<T> &v1);       \
    static VECTOR_G<T> Round(const VECTOR_G<T> &v1);      \
    static VECTOR_G<T> Clamp(const VECTOR_G<T> &v,        \
                             const VECTOR_G<T> &min,      \
                             const VECTOR_G<T> &max);     \
    static VECTOR_G<T> Clamp2(const VECTOR_G<T> &v,       \
                              const VECTOR_G<T> &bound1,  \
                              const VECTOR_G<T> &bound2); \
    \
    template<class Real> \
    static VECTOR_G<T> Lerp(const VECTOR_G<T> &v1, \
                            const VECTOR_G<T> &v2, \
                            Real t); \
    \
    Axis GetAxis() const; \
    const T& GetAxis(Axis axis) const; \
    static VECTOR_G<T> FromAxis(Axis axis); \
    \
    T& operator[](std::size_t i); \
    const T& operator[](std::size_t i) const; \
    \
    EXTRA_DECLARATIONS \
}; \
\
\
template<class T> \
bool operator==(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
bool operator!=(const VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> & v1, const VECTOR_G<T> &v2); \
\
template<class T> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
\
template<class T> \
VECTOR_G<T> operator*(const T& a, const VECTOR_G<T> &v); \
\
template<class T> \
VECTOR_G<T> operator*(const VECTOR_G<T> &v, const T& a); \
\
template<class T> \
VECTOR_G<T> operator/(const T& a, const VECTOR_G<T> &v); \
\
template<class T> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v, const T& a); \
\
template<class T> \
VECTOR_G<T> operator/(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
\
template<class T> \
VECTOR_G<T> &operator+=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
VECTOR_G<T> &operator-=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
VECTOR_G<T> &operator*=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
VECTOR_G<T> &operator/=(VECTOR_G<T> &lhs, const VECTOR_G<T> &rhs); \
\
template<class T> \
VECTOR_G<T> operator+(const T& a, const VECTOR_G<T> &v); \
\
template<class T> \
VECTOR_G<T> operator+(const VECTOR_G<T> &v, const T& a); \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v1, const VECTOR_G<T> &v2); \
\
template<class T> \
VECTOR_G<T> operator-(const T& a, const VECTOR_G<T> &v); \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v, const T& a); \
\
template<class T> \
VECTOR_G<T>& operator+=(VECTOR_G<T> &lhs, const T& a); \
\
template<class T> \
VECTOR_G<T>& operator-=(VECTOR_G<T> &lhs, const T& a); \
\
template<class T> \
VECTOR_G<T>& operator*=(VECTOR_G<T> &lhs, const T& a); \
\
template<class T> \
VECTOR_G<T>& operator/=(VECTOR_G<T> &lhs, const T& a); \
\
template<class T> \
VECTOR_G<T> operator-(const VECTOR_G<T> &v);

#include "Bang/Vector.tcc"

#endif // VECTOR_H
