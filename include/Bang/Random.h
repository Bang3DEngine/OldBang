#ifndef RANDOM_H
#define RANDOM_H

#include "Bang/Color.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/Vector4.h"
#include "Bang/Quaternion.h"

NAMESPACE_BANG_BEGIN

class Random
{
public:
    static void SetSeed(long seed);
    static void SetSeed(const String &seed);

    template<class T = float>
    static T GetValue();

    template<class T>
    static T GetRange(T minIncluded, T maxExcluded);

    static Vector2 GetInsideUnitCircle();
    static Vector3 GetInsideUnitSphere();
    static Vector4 GetRandomVector4();
    static Quaternion GetRotation();
    static Color GetColor();
    static Color GetColorOpaque();

private:
    Random();
};

template<class T>
T Random::GetValue()
{
    return float(rand()) / RAND_MAX;
}

template<class T>
T Random::GetRange(T minIncluded, T maxExcluded)
{
    return Cast<T>(Random::GetValue() * (maxExcluded-minIncluded)) + minIncluded;
}

NAMESPACE_BANG_END

#endif // RANDOM_H
