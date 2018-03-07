#include "Bang/Geometry.h"

#include "Bang/Ray.h"
#include "Bang/Plane.h"
#include "Bang/Sphere.h"
#include "Bang/Triangle.h"

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

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-
// rendering-a-triangle/ray-triangle-intersection-geometric-solution
void Geometry::RayTriangle(const Ray &ray,
                           const Triangle &triangle,
                           bool *intersected,
                           float *distanceFromRayOriginToIntersection)
{
    constexpr float EPSILON = 1e-5f;

    float &t = *distanceFromRayOriginToIntersection;
    const Vector3 &rayOrig = ray.GetOrigin();
    const Vector3 &rayDir  = ray.GetDirection();
    const Vector3 &v0 = triangle.GetPoint(0);
    const Vector3 &v1 = triangle.GetPoint(1);
    const Vector3 &v2 = triangle.GetPoint(2);

    // Compute plane's normal
    const Vector3 v0v1 = v1 - v0;
    const Vector3 v0v2 = v2 - v0;

    // No need to normalize
    const Vector3 triPlaneNormal = Vector3::Cross(v0v1, v0v2); // N

    // Step 1: finding P

    // Check if ray and plane are parallel ?
    const float NdotRayDirection = Vector3::Dot(triPlaneNormal, rayDir);
    if (Math::Abs(NdotRayDirection) < EPSILON) // Almost 0
    {
        *intersected = false; // Parallel so they don't intersect
        return;
    }

    // Compute d parameter using equation 2
    const float d = Vector3::Dot(triPlaneNormal, v0);

    // Compute t (equation 3)
    t = (Vector3::Dot(triPlaneNormal, rayOrig) + d) / NdotRayDirection;

    // Check if the triangle is in behind the ray
    if (t < 0) { *intersected = false; return; } // The triangle is behind

    // Compute the intersection point using equation 1
    const Vector3 P = rayOrig + t * rayDir;

    // Step 2: inside-outside test

    // Edge 0
    const Vector3 edge0 = v1 - v0;
    const Vector3   vp0 = P  - v0;
    const Vector3 C0 = Vector3::Cross(edge0, vp0); // Perp. to triangle's plane
    if (Vector3::Dot(triPlaneNormal, C0) < 0)
    {
        *intersected = false;
        return; // P is on the right side
    }

    // Edge 1
    const Vector3 edge1 = v2 - v1;
    const Vector3   vp1 = P  - v1;
    const Vector3 C1 = Vector3::Cross(edge1, vp1); // Perp. to triangle's plane
    if (Vector3::Dot(triPlaneNormal, C1) < 0)
    {
        *intersected = false;
        return; // P is on the right side
    }

    // Edge 2
    const Vector3 edge2 = v0 - v2;
    const Vector3   vp2 = P  - v2;
    const Vector3 C2 = Vector3::Cross(edge2, vp2); // Perp. to triangle's plane
    if (Vector3::Dot(triPlaneNormal, C2) < 0)
    {
        *intersected = false;
        return; // P is on the right side
    }

    *intersected = true; // Ray hits the triangle
}

void Geometry::RayTriangle(const Ray &ray,
                           const Triangle &triangle,
                           bool *intersected,
                           Vector3 *intersectionPoint)

{
    float t;
    Geometry::RayTriangle(ray, triangle, intersected, &t);
    *intersected       = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
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
