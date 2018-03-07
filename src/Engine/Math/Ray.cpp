#include "Bang/Ray.h"

#include <algorithm>

#include "Bang/Plane.h"
#include "Bang/Sphere.h"
#include "Bang/Geometry.h"

USING_NAMESPACE_BANG

Ray::Ray()
{
}

Ray::Ray(const Vector3 &origin, const Vector3 &direction)
{
    SetOrigin(origin);
    SetDirection(direction);
}

Ray::~Ray()
{
}

void Ray::SetOrigin(const Vector3 &origin) { m_origin = origin; }
void Ray::SetDirection(const Vector3 &direction)
{
    m_direction = direction.NormalizedSafe();
}

Vector3 Ray::GetPoint(float t) const
{
    return GetOrigin() + GetDirection() * t;
}

const Vector3 &Ray::GetOrigin() const
{
    return m_origin;
}

const Vector3 &Ray::GetDirection() const
{
    return m_direction;
}

