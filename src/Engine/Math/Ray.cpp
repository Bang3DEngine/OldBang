#include "Bang/Ray.h"

#include <algorithm>

#include "Bang/Plane.h"
#include "Bang/Sphere.h"

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

void Ray::GetIntersectionWithPlaneDistance(const Plane& plane,
                                           bool *intersected,
                                           float *distance)
{
    const Vector3& planeNormal = plane.GetNormal();
    float dot = Vector3::Dot(planeNormal, GetDirection());
    *intersected = (Math::Abs(dot) > 0.001f);

    if (*intersected)
    {
        *distance = Vector3::Dot(plane.GetPoint() - GetOrigin(), planeNormal) / dot;
    }
}

void Ray::GetIntersectionWithPlane(const Plane& plane,
                                   bool *intersected,
                                   Vector3 *intersectionPoint)
{
    float t;
    GetIntersectionWithPlaneDistance(plane, intersected, &t);
    *intersected = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? GetPoint(t) : GetOrigin();
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/
// minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
void Ray::GetIntersectionWithSphere(const Sphere &sphere,
                                    bool *intersected,
                                    Vector3 *intersectionPoint)
{
    float t0, t1; // solutions for t if the ray intersects

    const Vector3 rayOriginToSphereCenter = sphere.GetCenter() - GetOrigin();

    const float sphereRadius2 = sphere.GetRadius() * sphere.GetRadius();
    const float tca = Vector3::Dot(rayOriginToSphereCenter, GetDirection());
    // if (tca < 0)  { *intersected = false; return; }
    const float d2 = rayOriginToSphereCenter.SqLength() - tca * tca;
    if (d2 > sphereRadius2) { *intersected = false; return; }
    const float thc = Math::Sqrt(sphereRadius2 - d2);
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
        bool intersected;
        Vector3 planeBitangent = lineDirection.Normalized();
        Plane plane(linePoint, Vector3::Cross(lineToRayPerp, planeBitangent));
        GetIntersectionWithPlane(plane, &intersected, pointOnRay);
    }

    if (pointOnLine)
    {
        Ray lineRay(linePoint, lineDirection);
        Vector3 planeBitangent = GetDirection();
        Plane plane( GetOrigin(), Vector3::Cross(lineToRayPerp, planeBitangent) );

        float t;
        bool intersected;
        lineRay.GetIntersectionWithPlaneDistance(plane, &intersected, &t);
        *pointOnLine = linePoint + (t * lineDirection.Normalized());
    }
}

Vector3 Ray::GetClosestPointToPoint(const Vector3 &point)
{
    bool intersected;
    Vector3 intersection;
    GetIntersectionWithPlane(Plane(point, GetDirection()),
                             &intersected, &intersection);
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

