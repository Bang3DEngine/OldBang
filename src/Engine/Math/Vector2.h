#ifndef VECTOR2_H
#define VECTOR2_H

#include "Bang/Vector.h"

#define XYZ_MEMBERS x,y
#define EXTRA_DECLARATIONS \
template <class OtherT1, class OtherT2>\
explicit Vector2G(const OtherT1 &_x, \
                  const OtherT2 &_y) : x( SCAST<T>(_x) ),\
                                       y( SCAST<T>(_y) ) {}\
\
const static Vector2G Up; \
const static Vector2G Down; \
const static Vector2G Right; \
const static Vector2G Left; \
const static Vector2G Zero; \
const static Vector2G One;

CLASS_VECTOR_T(Vector2G, 2)

template<class T>
const Vector2G<T> Vector2G<T>::Up = Vector2G<T>(SCAST<T>(0), SCAST<T>(1));
template<class T>
const Vector2G<T> Vector2G<T>::Down = Vector2G<T>(SCAST<T>(0), SCAST<T>(-1));
template<class T>
const Vector2G<T> Vector2G<T>::Right = Vector2G<T>(SCAST<T>(1), SCAST<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::Left = Vector2G<T>(SCAST<T>(-1), SCAST<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::Zero = Vector2G<T>(SCAST<T>(0));
template<class T>
const Vector2G<T> Vector2G<T>::One = Vector2G<T>(SCAST<T>(1));

#undef XYZ_MEMBERS
#undef EXTRA_DECLARATIONS

#endif // VECTOR2_H
