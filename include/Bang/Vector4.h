#ifndef VECTOR4_H
#define VECTOR4_H

#include "Bang/VectorG.h"

template <class T>
class VectorG<4,T> : public VectorGCommon<4,T>
{
public:
    using Base = VectorGCommon<4,T>;
    T x, y, z, w;

    using Base::VectorGCommon;

    VectorG() : Base() {}
    VectorG (const Base &v) : VectorG(v.At(0), v.At(1), v.At(2), v.At(3))
    {
    }

    explicit VectorG(const T& _x, const T& _y, const T& _z, const T& _w)
                                                 : x( SCAST<T>(_x) ),
                                                   y( SCAST<T>(_y) ),
                                                   z( SCAST<T>(_z) ),
                                                   w( SCAST<T>(_w) )
    {
    }

    explicit VectorG(const Vector2G<T> &v, const T &_z, const T &_w)
                                                : x( SCAST<T>(v.x) ),
                                                  y( SCAST<T>(v.y) ),
                                                  z( SCAST<T>(_z) ),
                                                  w( SCAST<T>(_w) )
    {
    }

    explicit VectorG(const T &_x, const Vector2G<T> &v, const T &_w)
                                                : x( SCAST<T>(_x) ),
                                                  y( SCAST<T>(v.y) ),
                                                  z( SCAST<T>(v.z) ),
                                                  w( SCAST<T>(_w) )
    {
    }

    explicit VectorG(const T &_x, const T &_y, const Vector2G<T> &v)
                                                : x( SCAST<T>(_x) ),
                                                  y( SCAST<T>(_y) ),
                                                  z( SCAST<T>(v.z) ),
                                                  w( SCAST<T>(v.w) )
    {
    }

    explicit VectorG(const Vector3G<T> &v, const T &_w)
                                                : x( SCAST<T>(v.x) ),
                                                  y( SCAST<T>(v.y) ),
                                                  z( SCAST<T>(v.z) ),
                                                  w( SCAST<T>(_w) )
    {
    }

    explicit VectorG(const T &_x, const Vector3G<T> &v)
                                                : x( SCAST<T>(_x) ),
                                                  y( SCAST<T>(v.y) ),
                                                  z( SCAST<T>(v.z) ),
                                                  w( SCAST<T>(v.w) )
    {
    }


    Vector2G<T> xy()  const { return Vector2G<T>(x,y); }
    Vector3G<T> xyz() const { return Vector3G<T>(x,y,z); }

    const static VectorG<4,T> Up;
    const static VectorG<4,T> Down;
    const static VectorG<4,T> Right;
    const static VectorG<4,T> Left;
    const static VectorG<4,T> Zero;
    const static VectorG<4,T> One;
    const static VectorG<4,T> Forward;
    const static VectorG<4,T> Back;
};

template<class T>
const VectorG<4,T> VectorG<4,T>::Up = VectorG<4,T>(SCAST<T>(0),
                                                   SCAST<T>(1),
                                                   SCAST<T>(0),
                                                   SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::Down = VectorG<4,T>(SCAST<T>(0),
                                                     SCAST<T>(-1),
                                                     SCAST<T>(0),
                                                     SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::Right = VectorG<4,T>(SCAST<T>(1),
                                                      SCAST<T>(0),
                                                      SCAST<T>(0),
                                                      SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::Left = VectorG<4,T>(SCAST<T>(-1),
                                                     SCAST<T>(0),
                                                     SCAST<T>(0),
                                                     SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::Zero = VectorG<4,T>(SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::One = VectorG<4,T>(SCAST<T>(1));
template<class T>
const VectorG<4,T> VectorG<4,T>::Forward = VectorG<4,T>(SCAST<T>(0),
                                                        SCAST<T>(0),
                                                        SCAST<T>(-1),
                                                        SCAST<T>(0));
template<class T>
const VectorG<4,T> VectorG<4,T>::Back = VectorG<4,T>(SCAST<T>(0),
                                                     SCAST<T>(0),
                                                     SCAST<T>(1),
                                                     SCAST<T>(0));

#endif // VECTOR4_H
