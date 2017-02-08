#include "Random.h"

float Random::Range(float minIncluded, float maxIncluded)
{
    return Value() * (maxIncluded-minIncluded) + minIncluded;
}

int Random::Range(int minIncluded, int maxExcluded)
{
    return (rand() % (maxExcluded-minIncluded)) + minIncluded;
}

float Random::Value()
{
    return float(rand()) / RAND_MAX;
}

Vector2 Random::InsideUnitCircle()
{
    return Vector2(Random::Value(),
                   Random::Value());
}

Vector3 Random::InsideUnitSphere()
{
    return Vector3(Random::Value(),
                   Random::Value(),
                   Random::Value());
}

Vector4 Random::RandomVector4()
{
    return Vector4(Random::Value(),
                   Random::Value(),
                   Random::Value(),
                   Random::Value());
}

Quaternion Random::GetRotation()
{
    const float angle = Random::Range(0.0f, 360.0f);
    return Quaternion::AngleAxis(angle, Random::InsideUnitSphere());
}

Color Random::GetColor()
{
    return Color(Random::Value(),
                 Random::Value(),
                 Random::Value(),
                 Random::Value());
}

Color Random::GetColorOpaque()
{
    return Color(Random::Value(),
                 Random::Value(),
                 Random::Value(), 1.0f);
}

Random::Random()
{
}

