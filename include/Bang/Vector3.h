#ifndef VECTOR3_H
#define VECTOR3_H

#include "Bang/VectorG.h"

template <class T>
class VectorG<3,T> : public VectorGCommon<3,T>
{
public:
public:
    using Base = VectorGCommon<3,T>;
    T x, y, z;

    using Base::VectorGCommon;

    VectorG () {}
    VectorG (const Base &v) : VectorG(v.At(0), v.At(1), v.At(2)) {}

    explicit VectorG(const T &_x, const T &_y, const T&_z)
                                            : x( SCAST<T>(_x) ),
                                              y( SCAST<T>(_y) ),
                                              z( SCAST<T>(_z) )
    {
    }

    explicit VectorG(const Vector2G<T> &v, const T &_z)
                                            : x( SCAST<T>(v.x) ),
                                              y( SCAST<T>(v.y) ),
                                              z( SCAST<T>(_z) )
    {
    }

    explicit VectorG(const T &_x, const Vector2G<T> &v)
                                            : x( SCAST<T>(_x) ),
                                              y( SCAST<T>(v.y) ),
                                              z( SCAST<T>(v.z) ) {}

    VectorG ProjectedOnPlane(const VectorG& planeNormal,
                              const VectorG& planePoint) const
    {
        VectorG n = planeNormal.Normalized();
        return (*this) - n * VectorG<3,T>::Dot(n, *this - planePoint);
    }

    static VectorG<3,T> Cross(const VectorG<3,T> &v1, const VectorG<3,T> &v2)
    {
        return VectorG<3,T>(v1.y*v2.z - v1.z*v2.y,
                           v1.z*v2.x - v1.x*v2.z,
                           v1.x*v2.y - v1.y*v2.x);
    }

    static VectorG<3,T> Reflect(const VectorG<3,T> &incident,
                               const VectorG<3,T> &normal)
    {
        VectorG<3,T> n = normal.Normalized();
        return incident - 2 * ( VectorG<3,T>::Dot(incident, n) ) * n;
    }

    Vector2G<T> xy() const { return Vector2G<T>(x,y); }

    const static VectorG<3,T> Up;
    const static VectorG<3,T> Down;
    const static VectorG<3,T> Right;
    const static VectorG<3,T> Left;
    const static VectorG<3,T> Zero;
    const static VectorG<3,T> One;
    const static VectorG<3,T> Forward;
    const static VectorG<3,T> Back;
};

template<class T>
const VectorG<3,T> VectorG<3,T>::Up = VectorG<3,T>(SCAST<T>(0),
                                                SCAST<T>(1),
                                                SCAST<T>(0));
template<class T>
const VectorG<3,T> VectorG<3,T>::Down = VectorG<3,T>(SCAST<T>(0),
                                                  SCAST<T>(-1),
                                                  SCAST<T>(0));
template<class T>
const VectorG<3,T> VectorG<3,T>::Right = VectorG<3,T>(SCAST<T>(1),
                                                   SCAST<T>(0),
                                                   SCAST<T>(0));
template<class T>
const VectorG<3,T> VectorG<3,T>::Left = VectorG<3,T>(SCAST<T>(-1),
                                                  SCAST<T>(0),
                                                  SCAST<T>(0));
template<class T>
const VectorG<3,T> VectorG<3,T>::Zero = VectorG<3,T>(SCAST<T>(0));
template<class T>
const VectorG<3,T> VectorG<3,T>::One = VectorG<3,T>(SCAST<T>(1));
template<class T>
const VectorG<3,T> VectorG<3,T>::Forward = VectorG<3,T>(SCAST<T>(0),
                                                     SCAST<T>(0),
                                                     SCAST<T>(-1));
template<class T>
const VectorG<3,T> VectorG<3,T>::Back = VectorG<3,T>(SCAST<T>(0),
                                                  SCAST<T>(0),
                                                  SCAST<T>(1));

#endif // VECTOR3_H
