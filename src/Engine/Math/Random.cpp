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

Vector2 Random::GetInsideUnitCircle()
{
    return Vector2(Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f));
}

Vector3 Random::GetInsideUnitSphere()
{
    return Vector3(Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f));
}

Vector4 Random::GetRandomVector4()
{
    return Vector4(Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f),
                   Random::GetRange(-1.0f, 1.0f));
}

Quaternion Random::GetRotation()
{
    const float angle = Random::GetRange(0.0f, 360.0f);
    return Quaternion::AngleAxis(angle, Random::GetInsideUnitSphere());
}

Color Random::GetColor()
{
    return Color(Random::GetValue(),
                 Random::GetValue(),
                 Random::GetValue(),
                 Random::GetValue());
}

Color Random::GetColorOpaque()
{
    return Color(Random::GetValue(),
                 Random::GetValue(),
                 Random::GetValue(), 1.0f);
}

Random::Random()
{
}
