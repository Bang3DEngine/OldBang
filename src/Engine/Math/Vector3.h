#ifndef VECTOR3_H
#define VECTOR3_H

#include "Bang/Vector.h"

template<class T> class Vector2G;

#define XYZ_MEMBERS x,y,z
#define EXTRA_DECLARATIONS \
template <class OtherT1, class OtherT2, class OtherT3>\
explicit Vector3G(const OtherT1 &_x, \
                  const OtherT2 &_y, \
                  const OtherT3 &_z) : x( SCAST<T>(_x) ),\
                                       y( SCAST<T>(_y) ),\
                                       z( SCAST<T>(_z) ) {}\
\
template <class OtherT1, class OtherT2>\
explicit Vector3G(const Vector2G<OtherT1> &v, const OtherT2 &_z)\
                        : x( SCAST<T>(v.x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(_z) ) {}\
\
template <class OtherT1, class OtherT2>\
explicit Vector3G(const OtherT1 &_x, const Vector2G<OtherT2> &v)\
                        : x( SCAST<T>(_x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(v.z) ) {}\
\
\
\
Vector3G ProjectedOnPlane(const Vector3G& planeNormal, \
                          const Vector3G& planePoint) const \
{ \
    Vector3G n = planeNormal.Normalized(); \
    return (*this) - n * Vector3G<T>::Dot(n, *this - planePoint); \
} \
template<class OtherT1, class OtherT2> \
static Vector3G<T> Cross(const Vector3G<OtherT1> &v1, \
                         const Vector3G<OtherT2> &v2) \
{ \
    return Vector3G<T>(v1.y*v2.z - v1.z*v2.y,  \
                       v1.z*v2.x - v1.x*v2.z,  \
                       v1.x*v2.y - v1.y*v2.x); \
} \
template<class OtherT1, class OtherT2> \
static Vector3G<T> Reflect(const Vector3G<OtherT1> &incident, \
                           const Vector3G<OtherT2> &normal) \
{ \
    Vector3G<T> n = normal.Normalized(); \
    return incident - 2 * ( Vector3G<T>::Dot(incident, n) ) * n; \
} \
\
Vector2G<T> xy() const { return Vector2G<T>(x,y); } \
\
const static Vector3G Up; \
const static Vector3G Down; \
const static Vector3G Right; \
const static Vector3G Left; \
const static Vector3G Zero; \
const static Vector3G One; \
const static Vector3G Forward; \
const static Vector3G Back; \

CLASS_VECTOR_T(Vector3G, 3)

template<class T>
const Vector3G<T> Vector3G<T>::Up = Vector3G<T>(SCAST<T>(0),
                                                SCAST<T>(1),
                                                SCAST<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Down = Vector3G<T>(SCAST<T>(0),
                                                  SCAST<T>(-1),
                                                  SCAST<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Right = Vector3G<T>(SCAST<T>(1),
                                                   SCAST<T>(0),
                                                   SCAST<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Left = Vector3G<T>(SCAST<T>(-1),
                                                  SCAST<T>(0),
                                                  SCAST<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Zero = Vector3G<T>(SCAST<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::One = Vector3G<T>(SCAST<T>(1));
template<class T>
const Vector3G<T> Vector3G<T>::Forward = Vector3G<T>(SCAST<T>(0),
                                                     SCAST<T>(0),
                                                     SCAST<T>(-1));
template<class T>
const Vector3G<T> Vector3G<T>::Back = Vector3G<T>(SCAST<T>(0),
                                                  SCAST<T>(0),
                                                  SCAST<T>(1));

using Vector3f = Vector3G<float>;
using Vector3d = Vector3G<double>;
using Vector3i = Vector3G<int>;
using Vector3u = Vector3G<uint>;
using Vector3  = Vector3f;

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

#endif // VECTOR3_H
