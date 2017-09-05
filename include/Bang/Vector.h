#ifndef VECTOR_H
#define VECTOR_H

#include "Bang/VectorG.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

class Vector
{
public:

    template<uint N, class T, class Real>
    static VectorG<N,T> Lerp(const VectorG<N,T> &v1,
                             const VectorG<N,T> &v2,
                             Real t)
    {
        return v1 + (v2 - v1) * t;
    }

    template<uint N, class T>
    static VectorG<N,T> Abs(const VectorG<N,T> &v)
    {
        return v.Abs();
    }

    template<uint N, class T>
    static T Dot(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
    {
        T res = SCAST<T>(0);
        for (int i = 0; i < N; ++i) { res += v1[i] * v2[i]; }
        return res;
    }

    template<uint N, class T>
    static T Distance(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
    {
        return (v1 - v2).Length();
    }

    template<uint N, class T>
    static T SqDistance(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
    {
        return (v1 - v2).SqLength();
    }

    template<uint N, class T>
    static VectorG<N,T> Max(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
    {
        VectorG<N,T> res;
        for (int i = 0; i < N; ++i) { res[i] = Math::Max(v1[i], v2[i]); }
        return res;
    }

    template<uint N, class T>
    static VectorG<N,T> Min(const VectorG<N,T> &v1, const VectorG<N,T> &v2)
    {
        VectorG<N,T> res;
        for (int i = 0; i < N; ++i) { res[i] = Math::Min(v1[i], v2[i]); }
        return res;
    }

    template<uint N, class T>
    static VectorG<N,T> Floor(const VectorG<N,T> &v1)
    {
        VectorG<N,T> res;
        for (int i = 0; i < N; ++i) { res[i] = Math::Floor(v1[i]); }
        return res;
    }

    template<uint N, class T>
    static VectorG<N,T> Ceil(const VectorG<N,T> &v1)
    {
        VectorG<N,T> res;
        for (int i = 0; i < N; ++i) { res[i] = Math::Ceil(v1[i]); }
        return res;
    }

    template<uint N, class T>
    static VectorG<N,T> Round(const VectorG<N,T> &v1)
    {
        VectorG<N,T> res;
        for (int i = 0; i < N; ++i) { res[i] = Math::Round(v1[i]); }
        return res;
    }

    template<uint N, class T>
    static VectorG<N,T> Clamp(const VectorG<N,T> &v,
                              const VectorG<N,T> &min,
                              const VectorG<N,T> &max)
    {
        return Vector::Min( Vector::Max(v, max), min );
    }

    Vector() = delete;
};

#endif // VECTOR_H
