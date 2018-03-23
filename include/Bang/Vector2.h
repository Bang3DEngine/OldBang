#ifndef VECTOR2_H
#define VECTOR2_H

#include "Bang/Vector.h"

NAMESPACE_BANG_BEGIN

#define XYZ_MEMBERS x,y
#define EXTRA_DECLARATIONS \
template <class OtherT1, class OtherT2>\
explicit Vector2G(const OtherT1 &_x, \
                  const OtherT2 &_y) : x( Cast<T>(_x) ),\
                                       y( Cast<T>(_y) ) {}\
\
Vector2G<T> Perpendicular() const { return Vector2G<T>(-y,x); } \
static T Cross(const Vector2G<T> &v1, const Vector2G<T> &v2) \
{  \
    return (v1.x * v2.y) - (v1.y * v2.x); \
} \
\
\
const static Vector2G Up; \
const static Vector2G Down; \
const static Vector2G Right; \
const static Vector2G Left; \
const static Vector2G Zero; \
const static Vector2G One; \
const static Vector2G Infinity; \
const static Vector2G NInfinity;

CLASS_VECTOR_T(Vector2G, 2)
CLASS_VECTOR_T_IMPL(Vector2G, 2)

template<class T>
const Vector2G<T> Vector2G<T>::Up = Vector2G<T>(Cast<T>(0), Cast<T>(1));
template<class T>
const Vector2G<T> Vector2G<T>::Down = Vector2G<T>(Cast<T>(0), Cast<T>(-1));
template<class T>
const Vector2G<T> Vector2G<T>::Right = Vector2G<T>(Cast<T>(1), Cast<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::Left = Vector2G<T>(Cast<T>(-1), Cast<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::Zero = Vector2G<T>(Cast<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::One = Vector2G<T>(Cast<T>(1));
template<class T>
const Vector2G<T> Vector2G<T>::Infinity = Vector2G<T>(Math::Max<T>());
template<class T>
const Vector2G<T> Vector2G<T>::NInfinity = Vector2G<T>(Math::Min<T>());

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

NAMESPACE_BANG_END

#endif // VECTOR2_H
