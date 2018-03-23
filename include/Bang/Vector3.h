#ifndef VECTOR3_H
#define VECTOR3_H

#include "Bang/Axis.h"
#include "Bang/Vector.h"

NAMESPACE_BANG_BEGIN

FORWARD_T class Vector2G;

#define XYZ_MEMBERS x,y,z
#define EXTRA_DECLARATIONS \
template <class OtherT1, class OtherT2, class OtherT3>\
explicit Vector3G(const OtherT1 &_x, \
                  const OtherT2 &_y, \
                  const OtherT3 &_z) : x( Cast<T>(_x) ),\
                                       y( Cast<T>(_y) ),\
                                       z( Cast<T>(_z) ) {}\
\
template <class OtherT1, class OtherT2>\
explicit Vector3G(const Vector2G<OtherT1> &v, const OtherT2 &_z)\
                        : x( Cast<T>(v.x) ),\
                          y( Cast<T>(v.y) ),\
                          z( Cast<T>(_z) ) {}\
\
template <class OtherT1, class OtherT2>\
explicit Vector3G(const OtherT1 &_x, const Vector2G<OtherT2> &v)\
                        : x( Cast<T>(_x) ),\
                          y( Cast<T>(v.y) ),\
                          z( Cast<T>(v.z) ) {}\
\
\
\
Vector3G<T> ProjectedOnPlane(const Vector3G<T> &planeNormal, \
                             const Vector3G<T> &planePoint) const \
{ \
    Vector3G<T> n = planeNormal.Normalized(); \
    return (*this) - n * Vector3G<T>::Dot(n, *this - planePoint); \
} \
Vector3G<T> ProjectedOnVector(const Vector3G<T> &vector) const \
{ \
    return Vector3G<T>::Dot(*this, vector) * vector.Normalized(); \
} \
Vector2G<T> ProjectedOnAxis(Axis3D axis) const \
{ \
    switch (axis) \
    { \
        case Axis3D::X: return Vector2G<T>(y, z); \
        case Axis3D::Y: return Vector2G<T>(x, z); \
        case Axis3D::Z: return Vector2G<T>(x, y); \
    } \
    return Vector2G<T>::Zero; \
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
const static Vector3G Infinity; \
const static Vector3G NInfinity;

CLASS_VECTOR_T(Vector3G, 3)
CLASS_VECTOR_T_IMPL(Vector3G, 3)

template<class T>
const Vector3G<T> Vector3G<T>::Up = Vector3G<T>(Cast<T>(0),
                                                Cast<T>(1),
                                                Cast<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Down = Vector3G<T>(Cast<T>(0),
                                                  Cast<T>(-1),
                                                  Cast<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Right = Vector3G<T>(Cast<T>(1),
                                                   Cast<T>(0),
                                                   Cast<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Left = Vector3G<T>(Cast<T>(-1),
                                                  Cast<T>(0),
                                                  Cast<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::Zero = Vector3G<T>(Cast<T>(0));
template<class T>
const Vector3G<T> Vector3G<T>::One = Vector3G<T>(Cast<T>(1));
template<class T>
const Vector3G<T> Vector3G<T>::Forward = Vector3G<T>(Cast<T>(0),
                                                     Cast<T>(0),
                                                     Cast<T>(-1));
template<class T>
const Vector3G<T> Vector3G<T>::Back = Vector3G<T>(Cast<T>(0),
                                                  Cast<T>(0),
                                                  Cast<T>(1));
template<class T>
const Vector3G<T> Vector3G<T>::Infinity = Vector3G<T>(Math::Max<T>());
template<class T>
const Vector3G<T> Vector3G<T>::NInfinity = Vector3G<T>(Math::Min<T>());

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

NAMESPACE_BANG_END

#endif // VECTOR3_H
