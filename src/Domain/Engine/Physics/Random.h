#ifndef RANDOM_H
#define RANDOM_H

#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

class Random
{
public:
    static float Range(float minIncluded, float maxIncluded);
    static int   Range(int minIncluded, int maxExcluded);
    static float Value();
    static Vector2 InsideUnitCircle();
    static Vector3 InsideUnitSphere();
    static Vector4 RandomVector4();
    static Quaternion GetRotation();
    static Color GetColor();
    static Color GetColorOpaque();

private:
    Random();
};

#endif // RANDOM_H
