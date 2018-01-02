#include "Bang/Ray.h"

#include <algorithm>

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

void Ray::GetIntersectionWithPlaneDistance(const Vector3 &planePoint,
                                           const Vector3 &_planeNormal,
                                           bool *intersected,
                                           float *distance)
{
    Vector3 planeNormal = _planeNormal.Normalized();
    float dot = Vector3::Dot(planeNormal, GetDirection());
    *intersected = (Math::Abs(dot) > 0.001f);

    if (*intersected)
    {
        *distance = Vector3::Dot(planePoint - GetOrigin(), planeNormal) / dot;
    }
}

void Ray::GetIntersectionWithPlane(const Vector3 &planePoint,
                                   const Vector3 &planeNormal,
                                   bool *intersected,
                                   Vector3 *intersectionPoint)
{
    float t;
    GetIntersectionWithPlaneDistance(planePoint, planeNormal,
                                     intersected, &t);
    *intersected = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? GetPoint(t) : GetOrigin();
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/
// minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
void Ray::GetIntersectionWithSphere(const Vector3 &sphereCenter,
                                    float sphereRadius,
                                    bool *intersected,
                                    Vector3 *intersectionPoint)
{
    float t0, t1; // solutions for t if the ray intersects

    Vector3 rayOriginToSphereCenter = sphereCenter - GetOrigin();

    float sphereRadius2 = sphereRadius * sphereRadius;
    float tca = Vector3::Dot(rayOriginToSphereCenter, GetDirection());
    // if (tca < 0)  { *intersected = false; return; }
    float d2 = rayOriginToSphereCenter.SqLength() - tca * tca;
    if (d2 > sphereRadius2) { *intersected = false; return; }
    float thc = Math::Sqrt(sphereRadius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) { std::swap(t0, t1); }

    if (t0 < 0)
    {
        t0 = t1;
        if (t0 < 0) { *intersected = false; return; }
    }

    *intersected = true;
    *intersectionPoint = GetPoint(t0);
}

void Ray::GetClosestPointToLine(const Vector3 &linePoint,
                                const Vector3 &lineDirection,
                                Vector3 *pointOnRay,
                                Vector3 *pointOnLine)
{
    Vector3 lineToRayPerp = Vector3::Cross(GetDirection(), lineDirection).Normalized();

    if (pointOnRay)
    {
        Vector3 planeBitangent = lineDirection.Normalized();
        Vector3 planeNormal = Vector3::Cross(lineToRayPerp, planeBitangent);
        Vector3 planePoint = linePoint;

        bool intersected;
        GetIntersectionWithPlane(planePoint, planeNormal,
                                 &intersected, pointOnRay);
    }

    if (pointOnLine)
    {
        Ray lineRay(linePoint, lineDirection);
        Vector3 planeBitangent = GetDirection();
        Vector3 planeNormal = Vector3::Cross(lineToRayPerp, planeBitangent);
        Vector3 planePoint = GetOrigin();

        float t;
        bool intersected;
        lineRay.GetIntersectionWithPlaneDistance(planePoint, planeNormal,
                                                 &intersected, &t);
        *pointOnLine = linePoint + (t * lineDirection.Normalized());
    }
}

Vector3 Ray::GetClosestPointToPoint(const Vector3 &point)
{
    bool intersected;
    Vector3 intersection;
    GetIntersectionWithPlane(point, GetDirection(), &intersected, &intersection);
    return intersected ? intersection : GetOrigin();
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

