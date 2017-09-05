#ifndef VECTOR2_H
#define VECTOR2_H

#include "Bang/VectorG.h"

template <class T>
class VectorG<2,T> : public VectorGCommon<2,T>
{
public:
    using Base = VectorGCommon<2,T>;
    T x, y;

    const static VectorG<2,T> Up;
    const static VectorG<2,T> Down;
    const static VectorG<2,T> Right;
    const static VectorG<2,T> Left;
    const static VectorG<2,T> Zero;
    const static VectorG<2,T> One;

    using Base::VectorGCommon;
    explicit VectorG() : Base() {}
    VectorG (const Base &v) : VectorG(v.At(0), v.At(1)) {}
    explicit VectorG(const T &_x, const T &_y) : x( SCAST<T>(_x) ),
                                                 y( SCAST<T>(_y) )
    {
    }
};

template<class T>
const VectorG<2,T> VectorG<2,T>::Up = VectorG<2,T>(SCAST<T>(0), SCAST<T>(1));
template<class T>
const VectorG<2,T> VectorG<2,T>::Down = VectorG<2,T>(SCAST<T>(0), SCAST<T>(-1));
template<class T>
const VectorG<2,T> VectorG<2,T>::Right = VectorG<2,T>(SCAST<T>(1), SCAST<T>(0));
template<class T>
const VectorG<2,T> VectorG<2,T>::Left = VectorG<2,T>(SCAST<T>(-1), SCAST<T>(0));
template<class T>
const VectorG<2,T> VectorG<2,T>::Zero = VectorG<2,T>(SCAST<T>(0));
template<class T>
const VectorG<2,T> VectorG<2,T>::One = VectorG<2,T>(SCAST<T>(1));

#endif // VECTOR2_H
