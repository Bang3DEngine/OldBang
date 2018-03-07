#include "Bang/Plane.h"

USING_NAMESPACE_BANG

Plane::Plane(const Vector3 &point, const Vector3 &normal)
{
    SetPoint(point);
    SetNormal(normal);
}

void Plane::SetPoint(const Vector3 &point)
{
    m_point = point;
}

void Plane::SetNormal(const Vector3 &normal)
{
    m_normal = normal.NormalizedSafe();
}

const Vector3 &Plane::GetPoint() const
{
    return m_point;
}

const Vector3 &Plane::GetNormal() const
{
    return m_normal;
}

