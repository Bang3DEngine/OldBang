#include "Bang/Random.h"

USING_NAMESPACE_BANG

void Random::SetSeed(long seed)
{
    srand(seed);
}

void Random::SetSeed(const String &seedStr)
{
    long seed = 0;
    for (char c : seedStr) { seed += int(c); }
    Random::SetSeed(seed);
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
