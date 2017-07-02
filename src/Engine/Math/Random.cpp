#include "Bang/Random.h"

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
    return Vector2(Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f));
}

Vector3 Random::InsideUnitSphere()
{
    return Vector3(Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f));
}

Vector4 Random::RandomVector4()
{
    return Vector4(Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f),
                   Random::Range(-1.0f, 1.0f));
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

