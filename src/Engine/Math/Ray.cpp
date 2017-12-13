#include "Bang/Ray.h"

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

void Ray::SetOrigin(const Vector3 &origin)
{
    m_origin = origin;
}

void Ray::SetDirection(const Vector3 &direction)
{
    m_direction = direction.Normalized();
}

Vector3 Ray::GetIntersectionWithPlane(const Vector3 &_planeNormal,
                                      const Vector3 &planePoint)
{
    Vector3 planeNormal = _planeNormal.Normalized();
    float dot = Vector3::Dot(planeNormal, GetDirection());
    if (Math::Abs(dot) > 0.0001f)
    {
        float t = Vector3::Dot(planePoint - GetOrigin(), planeNormal) / dot;
        if (t >= 0) { return GetPoint(t); }
    }
    return Vector3( Math::NaN<float>() );
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

