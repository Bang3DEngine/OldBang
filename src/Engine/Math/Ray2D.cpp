#include "Bang/Ray2D.h"

USING_NAMESPACE_BANG

Ray2D::Ray2D()
{
}

Ray2D::Ray2D(const Vector2 &origin, const Vector2 &direction)
{
    SetOrigin(origin);
    SetDirection(direction);
}

Ray2D::~Ray2D()
{
}

void Ray2D::SetOrigin(const Vector2 &origin)
{
    m_origin = origin;
}

void Ray2D::SetDirection(const Vector2 &direction)
{
    m_direction = direction.NormalizedSafe();
}

Vector2 Ray2D::GetPoint(float t) const
{
    return GetOrigin() + GetDirection() * t;
}

const Vector2 &Ray2D::GetOrigin() const
{
    return m_origin;
}

const Vector2 &Ray2D::GetDirection() const
{
    return m_direction;
}

