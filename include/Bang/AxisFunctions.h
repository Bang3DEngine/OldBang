#ifndef AXISFUNCTIONS_H
#define AXISFUNCTIONS_H

#include "Bang/Axis.h"
#include "Bang/Color.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

Color GetColorFromAxis(Axis3D axis)
{
    switch (axis)
    {
        case Axis3D::X: return Color::Red;
        case Axis3D::Y: return Color::Green;
        case Axis3D::Z: return Color::Blue;
    }
    ASSERT(false);
}

Vector3 GetVectorFromAxis(Axis3D axis)
{
    switch (axis)
    {
        case Axis3D::X: return Vector3::Right;
        case Axis3D::Y: return Vector3::Up;
        case Axis3D::Z: return Vector3::Forward;
    }
    ASSERT(false);
}

NAMESPACE_BANG_END

#endif // AXISFUNCTIONS_H
