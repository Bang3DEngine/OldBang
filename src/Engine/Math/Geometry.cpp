#include "Bang/Geometry.h"

#include <array>

#include "Bang/Ray.h"
#include "Bang/Quad.h"
#include "Bang/Plane.h"
#include "Bang/Sphere.h"
#include "Bang/Triangle.h"

USING_NAMESPACE_BANG

void Geometry::IntersectRayPlane(const Ray &ray,
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

void Geometry::IntersectRayPlane(const Ray &ray,
                                 const Plane &plane,
                                 bool *intersected,
                                 Vector3 *intersectionPoint)
{
    float t;
    Geometry::IntersectRayPlane(ray, plane, intersected, &t);
    *intersected       = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/
// minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
void Geometry::IntersectRaySphere(const Ray &ray,
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
        Geometry::IntersectRayPlane(ray, plane, &intersected, pointOnRay);
    }

    if (pointOnLine)
    {
        Ray lineRay(linePoint, lineDirection);
        Vector3 planeBitangent = ray.GetDirection();
        Plane plane( ray.GetOrigin(),
                     Vector3::Cross(lineToRayPerp, planeBitangent) );

        float t;
        bool intersected;
        Geometry::IntersectRayPlane(lineRay, plane, &intersected, &t);
        *pointOnLine = linePoint + (t * lineDirection.NormalizedSafe());
    }
}

// http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
void Geometry::IntersectRayTriangle(const Ray &ray,
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

    Vector3 v1v0 = v1 - v0;
    Vector3 v2v0 = v2 - v0;

    Vector3 h = Vector3::Cross(rayDir, v2v0);
    float   a = Vector3::Dot(v1v0, h);

    if (a > -0.00001 && a < 0.00001) { *intersected = false; return; }

    float   f = 1.0f / a;
    Vector3 s = rayOrig - v0;
    float   u = f * Vector3::Dot(s, h);

    if (u < 0.0 || u > 1.0) { *intersected = false; return; }

    Vector3 q = Vector3::Cross(s, v1v0);
    float   v = f * Vector3::Dot(rayDir, q);

    if (v < 0.0 || u + v > 1.0)  { *intersected = false; return; }

    // At this stage we can compute t to find out where
    // the intersection point is on the line
    t = f * Vector3::Dot(v2v0, q);
    if (t < 0.00001)  { *intersected = false; return; } // Ray intersection

    *intersected = true;
}

void Geometry::IntersectRayTriangle(const Ray &ray,
                                    const Triangle &triangle,
                                    bool *intersected,
                                    Vector3 *intersectionPoint)
{
    float t;
    Geometry::IntersectRayTriangle(ray, triangle, intersected, &t);
    *intersected       = *intersected && (t >= 0.0f);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
}

void Geometry::IntersectSegmentTriangle(const Vector3 &segmentPoint0,
                                        const Vector3 &segmentPoint1,
                                        const Triangle &triangle,
                                        bool *intersected,
                                        Vector3 *intersectionPoint)
{
    Ray ray;
    ray.SetOrigin(segmentPoint0);
    ray.SetDirection(segmentPoint1 - segmentPoint0);

    float t;
    Geometry::IntersectRayTriangle(ray, triangle, intersected, &t);

    const float segmentLength = Vector3::Distance(segmentPoint0, segmentPoint1);
    *intersected = *intersected && (t >= 0.0f) && (t <= segmentLength);
    *intersectionPoint = *intersected ? ray.GetPoint(t) : ray.GetOrigin();
}

void Geometry::IntersectTriangleTriangle(const Triangle &triangle0,
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
            Geometry::IntersectSegmentTriangle(triangles[t].GetPoint( i ),
                                               triangles[t].GetPoint( (i+1) % 3 ),
                                               triangles[1-t],
                                               &intersected,
                                               &intersPoint);

            if (intersected)
            {
                if (*numIntersPoints == 0)
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
                        ++(*numIntersPoints);
                        break;
                    }
                }
            }
        }
    }
}

void Geometry::IntersectQuadQuad(const Quad &quad0,
                                 const Quad &quad1,
                                 int *numIntersectionPoints,
                                 Vector3 *intersectionPoint0,
                                 Vector3 *intersectionPoint1)
{
    // Forward to another function
    Triangle quad0Tri0, quad0Tri1, quad1Tri0, quad1Tri1;
    quad0.GetTriangles(&quad0Tri0, &quad0Tri1);
    quad1.GetTriangles(&quad1Tri0, &quad1Tri1);
    Geometry::IntersectQuadQuad(quad0Tri0,
                                quad0Tri1,
                                quad1Tri0,
                                quad1Tri1,
                                numIntersectionPoints,
                                intersectionPoint0,
                                intersectionPoint1);
}

Array<Vector3> Geometry::IntersectQuadQuad(const Quad &quad0, const Quad &quad1)
{
    int numInts;
    std::array<Vector3, 2> ps;
    Geometry::IntersectQuadQuad(quad0, quad1, &numInts, &ps[0], &ps[1]);

    Array<Vector3> intersectionPoints;
    for (int i = 0; i < numInts; ++i) { intersectionPoints.PushBack(ps[i]); }

    return intersectionPoints;
}

void Geometry::IntersectQuadQuad(const Triangle &quad0Tri0,
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
            Geometry::IntersectTriangleTriangle(triQuad0,
                                                triQuad1,
                                                &numSubIntersectionPoints,
                                                &intersPoint0,
                                                &intersPoint1);

            if (numSubIntersectionPoints >= 1)
            { foundIntersectionPoints.PushBack(intersPoint0); }

            if (numSubIntersectionPoints == 2)
            { foundIntersectionPoints.PushBack(intersPoint1); }

            // Found 2 different in same try, no need to continue
            if (numSubIntersectionPoints == 2) { break; }
        }
    }

    if ( foundIntersectionPoints.Size() == 0 ) { *numIntersPoints = 0; return; }
    if ( foundIntersectionPoints.Size() == 1 )
    {
        *numIntersPoints = 1;
        *intersPointOut0 = foundIntersectionPoints.Front();
        return;
    }

    // If more than one point, take only those points that are not repeated
    // There should be at most two not repeated
    if ( foundIntersectionPoints.Size() >= 2 )
    {
        *numIntersPoints = 1;
        *intersPointOut0 = foundIntersectionPoints.Front();
        for (const Vector3 &foundIntersectionPoint : foundIntersectionPoints)
        {
            // Pick second point only if it's not the same as the first one
            if (Vector3::Distance(*intersPointOut0, foundIntersectionPoint) > ALMOST_ZERO)
            {
                *numIntersPoints = 2;
                *intersPointOut1 = foundIntersectionPoint;
                break;
            }
        }
    }
}

Array<Vector3> Geometry::IntersectQuadQuad(const Triangle &quad0Tri0,
                                           const Triangle &quad0Tri1,
                                           const Triangle &quad1Tri0,
                                           const Triangle &quad1Tri1)
{
    int numInts;
    std::array<Vector3, 2> ps;
    Geometry::IntersectQuadQuad(quad0Tri0, quad0Tri1, quad1Tri0, quad1Tri1,
                                &numInts, &ps[0], &ps[1]);

    Array<Vector3> intersectionPoints;
    for (int i = 0; i < numInts; ++i) { intersectionPoints.PushBack(ps[i]); }

    return intersectionPoints;
}

void Geometry::IntersectQuadAABox(const Quad &quad,
                                  const AABox &aaBox,
                                  int *numIntersPoints,
                                  Vector3 *intersPointOut0,
                                  Vector3 *intersPointOut1,
                                  Vector3 *intersPointOut2,
                                  Vector3 *intersPointOut3)
{
    // Do all combinations of quad-quad, similar to QuadQuad
    Array<Vector3> foundIntersectionPoints;
    const std::array<Quad, 6> aaBoxQuads = aaBox.GetQuads();
    for (const Quad &aaBoxQuad : aaBoxQuads)
    {
        int numSubIntersectionPoints;
        Vector3 intersPoint0, intersPoint1;
        Geometry::IntersectQuadQuad(quad,
                           aaBoxQuad,
                           &numSubIntersectionPoints,
                           &intersPoint0,
                           &intersPoint1);

        if (numSubIntersectionPoints >= 1)
        { foundIntersectionPoints.PushBack(intersPoint0); }

        if (numSubIntersectionPoints == 2)
        { foundIntersectionPoints.PushBack(intersPoint1); }
    }

    // Take only the two different points
    if ( foundIntersectionPoints.IsEmpty() ) { *numIntersPoints = 0; return; }

    // We know that at most there will be 4 inters. points.
    // Pick only those unique points different.
    Array<Vector3> goodIntersectionPoints;
    for (const Vector3 &foundIntersectionPoint : foundIntersectionPoints)
    {
        if (!goodIntersectionPoints.Contains(foundIntersectionPoint))
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

Array<Vector3> Geometry::IntersectQuadAABox(const Quad &quad, const AABox &aaBox)
{
    int numInts;
    std::array<Vector3, 4> ps;
    Geometry::IntersectQuadAABox(quad, aaBox,
                                 &numInts, &ps[0], &ps[1], &ps[2], &ps[3]);

    Array<Vector3> intersectionPoints;
    for (int i = 0; i < numInts; ++i) { intersectionPoints.PushBack(ps[i]); }

    return intersectionPoints;
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
    Geometry::IntersectRayPlane(ray,
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
