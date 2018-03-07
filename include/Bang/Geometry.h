#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class Ray;
FORWARD class Plane;
FORWARD class Sphere;
FORWARD class Triangle;

class Geometry
{
public:
    // Computes the intersection between a ray and a plane.
    static void RayPlane(const Ray &ray,
                         const Plane& plane,
                         bool *intersected,
                         float *distanceFromIntersectionToRayOrigin);
    static void RayPlane(const Ray &ray,
                         const Plane& plane,
                         bool *intersected,
                         Vector3 *intersectionPoint);

    // Computes the intersection between a ray and a sphere
    static void RaySphere(const Ray &ray,
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
    static void RayTriangle(const Ray &ray,
                            const Triangle &triangle,
                            bool *intersected,
                            float *distanceFromRayOriginToIntersection);
    static void RayTriangle(const Ray &ray,
                            const Triangle &triangle,
                            bool *intersected,
                            Vector3 *intersectionPoint);

    // Returns the point in the ray that is closer to the passed point
    static Vector3 RayClosestPointTo(const Ray &ray, const Vector3 &point);

    // Returns a point projected to sphere
    static Vector3 PointProjectedToSphere(const Vector3 &point,
                                          const Sphere &sphere);

    Geometry() = delete;
    virtual ~Geometry() = delete;
};

NAMESPACE_BANG_END

#endif // GEOMETRY_H

