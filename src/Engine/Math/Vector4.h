#ifndef VECTOR4_H
#define VECTOR4_H

#include "Bang/Vector.h"

template<class T> class Vector2G;
template<class T> class Vector3G;

#define XYZ_MEMBERS x,y,z,w
#define EXTRA_DECLARATIONS \
template <class OtherT1, class OtherT2, class OtherT3, class OtherT4>\
explicit Vector4G(OtherT1 _x, \
                  OtherT2 _y, \
                  OtherT3 _z, \
                  OtherT4 _w) : \
                                       x( SCAST<T>(_x) ),\
                                       y( SCAST<T>(_y) ),\
                                       z( SCAST<T>(_z) ),\
                                       w( SCAST<T>(_w) ) {}\
\
template <class OtherT1, class OtherT2, class OtherT3>\
explicit Vector4G(const Vector2G<OtherT1> &v, const OtherT2 &_z, const OtherT3 &_w)\
                        : x( SCAST<T>(v.x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(_z) ),\
                          w( SCAST<T>(_w) ) {}\
template <class OtherT1, class OtherT2, class OtherT3>\
explicit Vector4G(const OtherT1 &_x, const Vector2G<OtherT2> &v, const OtherT3 &_w)\
                        : x( SCAST<T>(_x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(v.z) ),\
                          w( SCAST<T>(_w) ) {}\
template <class OtherT1, class OtherT2, class OtherT3>\
explicit Vector4G(const OtherT1 &_x, const OtherT2 &_y, const Vector2G<OtherT3> &v)\
                        : x( SCAST<T>(_x) ),\
                          y( SCAST<T>(_y) ),\
                          z( SCAST<T>(v.z) ),\
                          w( SCAST<T>(v.w) ) {}\
template <class OtherT1, class OtherT2>\
explicit Vector4G(const Vector3G<OtherT1> &v, const OtherT2 &_w)\
                        : x( SCAST<T>(v.x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(v.z) ),\
                          w( SCAST<T>(_w) ) {}\
template <class OtherT1, class OtherT2>\
explicit Vector4G(const OtherT1 &_x, const Vector3G<OtherT2> &v)\
                        : x( SCAST<T>(_x) ),\
                          y( SCAST<T>(v.y) ),\
                          z( SCAST<T>(v.z) ),\
                          w( SCAST<T>(v.w) ) {}\
\
\
Vector2G<T> xy()  const { return Vector2G<T>(x,y); } \
Vector3G<T> xyz() const { return Vector3G<T>(x,y,z); } \
\
const static Vector4G Up; \
const static Vector4G Down; \
const static Vector4G Right; \
const static Vector4G Left; \
const static Vector4G Zero; \
const static Vector4G One; \
const static Vector4G Forward; \
const static Vector4G Back; \

CLASS_VECTOR_T(Vector4G, 4)

template<class T>
const Vector4G<T> Vector4G<T>::Up = Vector4G<T>(SCAST<T>(0),
                                                SCAST<T>(1),
                                                SCAST<T>(0),
                                                SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::Down = Vector4G<T>(SCAST<T>(0),
                                                  SCAST<T>(-1),
                                                  SCAST<T>(0),
                                                  SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::Right = Vector4G<T>(SCAST<T>(1),
                                                   SCAST<T>(0),
                                                   SCAST<T>(0),
                                                   SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::Left = Vector4G<T>(SCAST<T>(-1),
                                                  SCAST<T>(0),
                                                  SCAST<T>(0),
                                                  SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::Zero = Vector4G<T>(SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::One = Vector4G<T>(SCAST<T>(1));
template<class T>
const Vector4G<T> Vector4G<T>::Forward = Vector4G<T>(SCAST<T>(0),
                                                     SCAST<T>(0),
                                                     SCAST<T>(-1),
                                                     SCAST<T>(0));
template<class T>
const Vector4G<T> Vector4G<T>::Back = Vector4G<T>(SCAST<T>(0),
                                                  SCAST<T>(0),
                                                  SCAST<T>(1),
                                                  SCAST<T>(0));

using Vector4f = Vector4G<float>;
using Vector4d = Vector4G<double>;
using Vector4i = Vector4G<int>;
using Vector4u = Vector4G<uint>;
using Vector4  = Vector4f;

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS



#endif // VECTOR4_H
