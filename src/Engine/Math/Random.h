#ifndef RANDOM_H
#define RANDOM_H

#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Quaternion.h"

class Random
{
public:
    template<class T = float>
    static T Value();

    template<class T>
    static T Range(T minIncluded, T maxExcluded);

    static Vector2 InsideUnitCircle();
    static Vector3 InsideUnitSphere();
    static Vector4 RandomVector4();
    static Quaternion GetRotation();
    static Color GetColor();
    static Color GetColorOpaque();

private:
    Random();
};

template<class T>
T Random::Value()
{
    return float(rand()) / RAND_MAX;
}

template<class T>
T Random::Range(T minIncluded, T maxExcluded)
{
    return SCAST<T>(Random::Value() * (maxExcluded-minIncluded)) + minIncluded;
}

#endif // RANDOM_H
