#ifndef AXISFUNCTIONS_H
#define AXISFUNCTIONS_H

#include "Bang/Axis.h"
#include "Bang/Color.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

inline int GetAxisIndex(Axis3D axis)
{
    switch (axis)
    {
        case Axis3D::X: return 0;
        case Axis3D::Y: return 1;
        case Axis3D::Z: return 2;
    }
    ASSERT(false);
    return -1;
}
inline int GetAxisIndex(Axis3DExt axis)
{
    switch (axis)
    {
        case Axis3DExt::X: case Axis3DExt::Y: case Axis3DExt::Z:
                             return GetAxisIndex( SCAST<Axis3D>(axis) );
        case Axis3DExt::XY:  return 3;
        case Axis3DExt::XZ:  return 4;
        case Axis3DExt::YZ:  return 5;
        case Axis3DExt::XYZ: return 6;
    }
    ASSERT(false);
    return -1;
}

inline std::pair<int,int> GetOtherAxisIndex(Axis3D axis)
{
    int i = GetAxisIndex(axis);
    return std::make_pair( (i+2) % 3, (i+2) % 3 );
}

inline Color GetAxisColor(Axis3D axis)
{
    switch (axis)
    {
        case Axis3D::X:   return Color::Red;
        case Axis3D::Y:   return Color::Green;
        case Axis3D::Z:   return Color::Blue;
    }
    ASSERT(false);
    return Color::White;
}
inline Color GetAxisColor(Axis3DExt axis)
{
    switch (axis)
    {
        case Axis3DExt::X: case Axis3DExt::Y: case Axis3DExt::Z:
                             return GetAxisColor( SCAST<Axis3D>(axis) );
        case Axis3DExt::XY:  return Color(1,1,0,1);
        case Axis3DExt::XZ:  return Color(1,0,1,1);
        case Axis3DExt::YZ:  return Color(0,1,1,1);
        case Axis3DExt::XYZ: return Color(1,1,1,1);
    }
    ASSERT(false);
    return Color::White;
}

inline Vector3 GetAxisVector(Axis3D axis)
{
    switch (axis)
    {
        case Axis3D::X: return Vector3::Right;
        case Axis3D::Y: return Vector3::Up;
        case Axis3D::Z: return Vector3::Forward;
    }
    ASSERT(false);
    return Vector3::Zero;
}
inline Vector3 GetAxisVector(Axis3DExt axis)
{
    switch (axis)
    {
        case Axis3DExt::X: case Axis3DExt::Y: case Axis3DExt::Z:
                             return GetAxisVector( SCAST<Axis3D>(axis) );
        case Axis3DExt::XY:  return Vector3(1,1,0);
        case Axis3DExt::XZ:  return Vector3(1,0,1);
        case Axis3DExt::YZ:  return Vector3(0,1,1);
        case Axis3DExt::XYZ: return Vector3(1,1,1);
    }
    ASSERT(false);
    return Vector3::Zero;
}

NAMESPACE_BANG_END

#endif // AXISFUNCTIONS_H
