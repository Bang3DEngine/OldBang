#include "Bang/Geometry.h"

#include "Bang/Ray.h"
#include "Bang/Plane.h"
#include "Bang/Sphere.h"

USING_NAMESPACE_BANG

void Geometry::RayPlane(const Ray &ray,
                        const Plane &plane,
                        bool *intersected,
                        float *distanceFromIntersectionToRayOrigin)
{
    const Vector3& planeNormal = plane.GetNormal();
    float dot = Vector3::Dot(planeNormal, ray.GetDirection());
    *intersected = (Math::Abs(dot) > 0.001f);

    if (*intersected)
    {
        *distanceFromIntersectionToRayOrigin =
                Vector3::Dot(plane.GetPoint() - ray.GetOrigin(),
                             planeNormal) / dot;
    }
}

void Geometry::RayPlane(const Ray &ray,
                        const Plane &plane,
                        bool *intersected,
                        Vector3 *intersectionPoint)
{
    float t;
    Geometry::RayPlane(ray, plane, intersected, &t);
    *intersected       = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/
// minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
void Geometry::RaySphere(const Ray &ray,
                         const Sphere &sphere,
                         bool *intersected,
                         Vector3 *intersectionPoint)
{
    const Vector3 rayOriginToSphereCenter = sphere.GetCenter() - ray.GetOrigin();

    const float sphereRadius2 = sphere.GetRadius() * sphere.GetRadius();
    const float tca = Vector3::Dot(rayOriginToSphereCenter, ray.GetDirection());
    // if (tca < 0)  { *intersected = false; return; }

    const float d2 = rayOriginToSphereCenter.SqLength() - tca * tca;
    if (d2 > sphereRadius2) { *intersected = false; return; }

    const float thc = Math::Sqrt(sphereRadius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 > t1) { std::swap(t0, t1); }

    if (t0 < 0)
    {
        t0 = t1;
        if (t0 < 0) { *intersected = false; return; }
    }

    *intersected = true;
    *intersectionPoint = ray.GetPoint(t0);
}

void Geometry::RayLineClosestPoints(const Ray &ray,
                                    const Vector3 &linePoint,
                                    const Vector3 &lineDirection,
                                    Vector3 *pointOnRay,
                                    Vector3 *pointOnLine)
{
    Vector3 lineToRayPerp = Vector3::Cross(ray.GetDirection(), lineDirection).
                            NormalizedSafe();

    if (pointOnRay)
    {
        bool intersected;
        Vector3 planeBitangent = lineDirection.NormalizedSafe();
        Plane plane(linePoint, Vector3::Cross(lineToRayPerp, planeBitangent));
        Geometry::RayPlane(ray, plane, &intersected, pointOnRay);
    }

    if (pointOnLine)
    {
        Ray lineRay(linePoint, lineDirection);
        Vector3 planeBitangent = ray.GetDirection();
        Plane plane( ray.GetOrigin(),
                     Vector3::Cross(lineToRayPerp, planeBitangent) );

        float t;
        bool intersected;
        Geometry::RayPlane(lineRay, plane, &intersected, &t);
        *pointOnLine = linePoint + (t * lineDirection.NormalizedSafe());
    }
}

Vector3 Geometry::RayClosestPointTo(const Ray &ray, const Vector3 &point)
{
    bool intersected;
    Vector3 intersection;
    Geometry::RayPlane(ray,
                            Plane(point, ray.GetDirection()),
                            &intersected,
                            &intersection);
    return intersected ? intersection : ray.GetOrigin();
}

Vector3 Geometry::PointProjectedToSphere(const Vector3 &point,
                                         const Sphere &sphere)
{
    Vector3 closestRayPointToSphereV   = sphere.GetCenter() - point;
    Vector3 closestRayPointToSphereDir = closestRayPointToSphereV.Normalized();
    return sphere.GetCenter() - closestRayPointToSphereDir * sphere.GetRadius();
}
