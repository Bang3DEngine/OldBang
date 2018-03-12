#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Ray;
FORWARD class Quad;
FORWARD class Plane;
FORWARD class Sphere;
FORWARD class Triangle;

class Geometry
{
public:
    enum Orientation { Left, Middle, Right };

    // Computes the intersection between a ray and a plane.
    static void IntersectRayPlane(const Ray &ray,
                                  const Plane& plane,
                                  bool *intersected,
                                  float *distanceFromIntersectionToRayOrigin);
    static void IntersectRayPlane(const Ray &ray,
                                  const Plane& plane,
                                  bool *intersected,
                                  Vector3 *intersectionPoint);

    // Computes the intersection between a ray and a sphere
    static void IntersectRaySphere(const Ray &ray,
                                   const Sphere& sphere,
                                   bool *intersected,
                                   Vector3 *intersectionPoint);

    // Returns the two closest points between the ray and the passed line
    static void RayLineClosestPoints(const Ray &ray,
                                     const Vector3 &linePoint,
                                     const Vector3 &lineDirection,
                                     Vector3 *pointOnRay  = nullptr,
                                     Vector3 *pointOnLine = nullptr);

    // Computes the intersection between a ray and a triangle
    static void IntersectRayTriangle(const Ray &ray,
                                     const Triangle &triangle,
                                     bool *intersected,
                                     float *distanceFromRayOriginToIntersection);
    static void IntersectRayTriangle(const Ray &ray,
                                     const Triangle &triangle,
                                     bool *intersected,
                                     Vector3 *intersectionPoint);

    // Computes the intersection between a triangle and a segment
    static void IntersectSegmentTriangle(const Vector3 &segmentPoint0,
                                         const Vector3 &segmentPoint1,
                                         const Triangle &triangle,
                                         bool *intersected,
                                         Vector3 *intersectionPoint);

    // Computes the intersection between a triangle and a triangle
    static void IntersectTriangleTriangle(const Triangle &triangle0,
                                          const Triangle &triangle1,
                                          int *numIntersectionPoints,
                                          Vector3 *intersectionPoint0,
                                          Vector3 *intersectionPoint1);
    static Array<Vector3> IntersectTriangleTriangle(const Triangle &triangle0,
                                                    const Triangle &triangle1);

    // Computes the intersection between a quad and a quad
    static void IntersectQuadQuad(const Quad &quad0,
                                  const Quad &quad1,
                                  int *numIntersectionPoints,
                                  Vector3 *intersectionPoint0,
                                  Vector3 *intersectionPoint1);
    static Array<Vector3> IntersectQuadQuad(const Quad &quad0,
                                            const Quad &quad1);
    static void IntersectQuadQuad(const Triangle &quad0Tri0,
                                  const Triangle &quad0Tri1,
                                  const Triangle &quad1Tri0,
                                  const Triangle &quad1Tri1,
                                  int *numIntersectionPoints,
                                  Vector3 *intersectionPoint0,
                                  Vector3 *intersectionPoint1);
    static Array<Vector3> IntersectQuadQuad(const Triangle &quad0Tri0,
                                            const Triangle &quad0Tri1,
                                            const Triangle &quad1Tri0,
                                            const Triangle &quad1Tri1);

    // Computes the intersection between a quad and a AABox
    static void IntersectQuadAABox(const Quad &quad,
                                   const AABox &aaBox,
                                   int *numIntersectionPoints,
                                   Vector3 *intersectionPoint0,
                                   Vector3 *intersectionPoint1,
                                   Vector3 *intersectionPoint2,
                                   Vector3 *intersectionPoint3);
    static Array<Vector3> IntersectQuadAABox(const Quad &quad,
                                             const AABox &aaBox);


    // Returns the orientation of a point vs a line
    static Orientation GetOrientation(const Vector3 &lineP0,
                                      const Vector3 &lineP1,
                                      const Vector3 &point);

    // Returns the point in the ray that is closer to the passed point
    static Vector3 RayClosestPointTo(const Ray &ray, const Vector3 &point);

    // Returns a point projected to sphere
    static Vector3 PointProjectedToSphere(const Vector3 &point,
                                          const Sphere &sphere);

    Geometry() = delete;
    virtual ~Geometry() = delete;

private:
    static constexpr float ALMOST_ZERO = 1e-5f;
};

NAMESPACE_BANG_END

#endif // GEOMETRY_H

