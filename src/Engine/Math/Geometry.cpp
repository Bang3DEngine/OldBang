#include "Bang/Geometry.h"

#include <array>

#include "Bang/Ray.h"
#include "Bang/Quad.h"
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
    if (Math::Abs(NdotRayDirection) < ALMOST_ZERO) // Almost 0
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

void Geometry::SegmentTriangle(const Vector3 &segmentPoint0,
                               const Vector3 &segmentPoint1,
                               const Triangle &triangle,
                               bool *intersected,
                               Vector3 *intersectionPoint)
{
    Ray ray;
    ray.SetOrigin(segmentPoint0);
    ray.SetDirection( segmentPoint1 - segmentPoint0 );

    float t;
    Geometry::RayTriangle(ray, triangle, intersected, &t);

    const float segmentLength = Vector3::Distance(segmentPoint0, segmentPoint1);
    *intersected = *intersected && (t >= 0.0f) && (t <= segmentLength);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
}

void Geometry::TriangleTriangle(const Triangle &triangle0,
                                const Triangle &triangle1,
                                int *numIntersPoints,
                                Vector3 *intersPoint0,
                                Vector3 *intersPoint1)
{
    // Do all combinations of segment-tri

    *numIntersPoints = 0;
    std::array<Triangle, 2> triangles = {{triangle0, triangle1}};
    for (int t : {0,1})
    {
        for (int i : {0,1,2})
        {
            if (*numIntersPoints == 2) { break; }

            bool intersected;
            Vector3 intersPoint;
            Geometry::SegmentTriangle(triangles[t].GetPoint( i ),
                                      triangles[t].GetPoint( (i+1) % 3 ),
                                      triangles[1-t],
                                      &intersected,
                                      &intersPoint);

            if (intersected)
            {
                if (*numIntersPoints == 1)
                {
                    *intersPoint0 = intersPoint;
                    ++(*numIntersPoints);
                }
                else
                {
                    // Check that this point is not the same as before, can
                    // happen in some cases, and we would be missing one point
                    if (Vector3::Distance(intersPoint, *intersPoint0) > ALMOST_ZERO)
                    {
                        *intersPoint1 = intersPoint;
                        break;
                    }
                }
            }
        }
    }
}

void Geometry::QuadQuad(const Quad &quad0,
                        const Quad &quad1,
                        int *numIntersectionPoints,
                        Vector3 *intersectionPoint0,
                        Vector3 *intersectionPoint1)
{
    // Forward to another function
    Triangle quad0Tri0, quad0Tri1, quad1Tri0, quad1Tri1;
    quad0.GetTriangles(&quad0Tri0, &quad0Tri1);
    quad1.GetTriangles(&quad1Tri0, &quad1Tri1);
    Geometry::QuadQuad(quad0Tri0,
                       quad0Tri1,
                       quad1Tri0,
                       quad1Tri1,
                       numIntersectionPoints,
                       intersectionPoint0,
                       intersectionPoint1);
}

void Geometry::QuadQuad(const Triangle &quad0Tri0,
                        const Triangle &quad0Tri1,
                        const Triangle &quad1Tri0,
                        const Triangle &quad1Tri1,
                        int *numIntersPoints,
                        Vector3 *intersPointOut0,
                        Vector3 *intersPointOut1)
{
    // Do all combinations of tri-tri, similar to TriangleTriangle

    *numIntersPoints = 0;
    Array<Vector3> foundIntersectionPoints;
    std::array<Triangle, 2> trianglesQuad0 = {{quad0Tri0, quad0Tri1}};
    std::array<Triangle, 2> trianglesQuad1 = {{quad1Tri0, quad1Tri1}};
    for (int t0 : {0,1})
    {
        for (int t1 : {0,1})
        {
            const Triangle &triQuad0 = trianglesQuad0[t0];
            const Triangle &triQuad1 = trianglesQuad1[t1];

            int numSubIntersectionPoints;
            Vector3 intersPoint0, intersPoint1;
            Geometry::TriangleTriangle(triQuad0,
                                       triQuad1,
                                       &numSubIntersectionPoints,
                                       &intersPoint0,
                                       &intersPoint1);

            if (numSubIntersectionPoints >= 1)
            { foundIntersectionPoints.PushBack(intersPoint0); }

            if (numSubIntersectionPoints == 2)
            { foundIntersectionPoints.PushBack(intersPoint1); }

            // Found 2 different, no need to continue
            if (numSubIntersectionPoints == 2) { break; }
        }
    }

    // Take only the two different points
    if ( foundIntersectionPoints.IsEmpty() ) { return; }

    if ( foundIntersectionPoints.Size() >= 1 )
    {
        *numIntersPoints = 1;
        *intersPointOut0 = foundIntersectionPoints[0];
    }

    if ( foundIntersectionPoints.Size() >= 2 )
    {
        *numIntersPoints = 2;
        *intersPointOut1 = foundIntersectionPoints[1];
    }

    // This happens in degenerate cases, where two or more quad segments intersect.
    // We know that at most there will be 2 inters. points, so some of them must
    // be repeated. Consequently, just pick the two different points of all.
    if ( foundIntersectionPoints.Size() >= 3 )
    {
        for (const Vector3 &foundIntersectionPoint : foundIntersectionPoints)
        {
            // Pick second point only if it's not the same as the first one
            if (Vector3::Distance(*intersPointOut0, foundIntersectionPoint) <
                ALMOST_ZERO)
            {
                *intersPointOut1 = foundIntersectionPoint;
                break;
            }
        }
    }
}

void Geometry::QuadAABox(const Quad &quad,
                         const AABox &aaBox,
                         int *numIntersPoints,
                         Vector3 *intersPointOut0,
                         Vector3 *intersPointOut1,
                         Vector3 *intersPointOut2,
                         Vector3 *intersPointOut3)
{
    // Do all combinations of quad-quad, similar to QuadQuad
    *numIntersPoints = 0;
    Array<Vector3> foundIntersectionPoints;
    const std::array<Quad, 6> aaBoxQuads = aaBox.GetQuads();
    for (const Quad &aaBoxQuad : aaBoxQuads)
    {
        int numSubIntersectionPoints;
        Vector3 intersPoint0, intersPoint1;
        Geometry::QuadQuad(quad,
                           aaBoxQuad,
                           &numSubIntersectionPoints,
                           &intersPoint0,
                           &intersPoint1);

        if (numSubIntersectionPoints >= 1)
        { foundIntersectionPoints.PushBack(intersPoint0); }

        if (numSubIntersectionPoints == 2)
        { foundIntersectionPoints.PushBack(intersPoint1); }

        // Found 2 different, no need to continue
        if (numSubIntersectionPoints == 2) { break; }
    }

    // Take only the two different points
    if ( foundIntersectionPoints.IsEmpty() ) { return; }

    if ( foundIntersectionPoints.Size() >= 1 )
    {
        *numIntersPoints = 1;
        *intersPointOut0 = foundIntersectionPoints[0];
    }

    if ( foundIntersectionPoints.Size() >= 2 )
    {
        *numIntersPoints = 2;
        *intersPointOut1 = foundIntersectionPoints[1];
    }

    if ( foundIntersectionPoints.Size() >= 3 )
    {
        *numIntersPoints = 3;
        *intersPointOut2 = foundIntersectionPoints[2];
    }

    if ( foundIntersectionPoints.Size() >= 4 )
    {
        *numIntersPoints = 4;
        *intersPointOut3 = foundIntersectionPoints[3];
    }


    // This happens in degenerate cases, where two or more quad segments intersect.
    // We know that at most there will be 4 inters. points, so some of them must
    // be repeated. Consequently, just pick the four different points of all.
    if ( foundIntersectionPoints.Size() >= 5 )
    {
        Array<Vector3> goodIntersectionPoints;
        goodIntersectionPoints.PushBack(*intersPointOut0);
        for (const Vector3 &foundIntersectionPoint : foundIntersectionPoints)
        {
            bool repeated = false;
            for (const Vector3 &goodIntersectionPoint : goodIntersectionPoints)
            {
                // Pick point only if it's not repeated as the first one
                if (Vector3::Distance(*intersPointOut0, foundIntersectionPoint) <
                    ALMOST_ZERO)
                {
                    repeated = true;
                    break;
                }
            }

            if (!repeated)
            {
                goodIntersectionPoints.PushBack(foundIntersectionPoint);
            }

            if (goodIntersectionPoints.Size() == 4) { break; }
        }

        *numIntersPoints = goodIntersectionPoints.Size();
        if (goodIntersectionPoints.Size() >= 1)
        { *intersPointOut0 = goodIntersectionPoints[0]; }
        if (goodIntersectionPoints.Size() >= 2)
        { *intersPointOut1 = goodIntersectionPoints[1]; }
        if (goodIntersectionPoints.Size() >= 3)
        { *intersPointOut2 = goodIntersectionPoints[2]; }
        if (goodIntersectionPoints.Size() >= 4)
        { *intersPointOut3 = goodIntersectionPoints[3]; }
    }
}

Geometry::Orientation Geometry::GetOrientation(const Vector3 &lineP0,
                                               const Vector3 &lineP1,
                                               const Vector3 &point)
{
    float det = ((point.x - lineP0.x) * (lineP1.y - lineP0.y)) -
                ((point.y - lineP0.y) * (lineP1.x - lineP0.x));
    if (Math::Abs(det) < ALMOST_ZERO) { return Orientation::Middle; }
    return (det < 0) ? Orientation::Left : Orientation::Right;
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
