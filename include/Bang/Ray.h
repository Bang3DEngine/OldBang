#ifndef RAY_H
#define RAY_H

#include "Bang/Bang.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

FORWARD class Plane;
FORWARD class Sphere;

class Ray
{
public:
	Ray();
    Ray(const Vector3 &origin, const Vector3 &direction);
    ~Ray();

    void SetOrigin(const Vector3 &origin);
    void SetDirection(const Vector3 &direction);

    void GetIntersectionWithPlaneDistance(const Plane& plane,
                                          bool *intersected,
                                          float *distance);
    void GetIntersectionWithPlane(const Plane& plane,
                                  bool *intersected,
                                  Vector3 *intersectionPoint);
    void GetIntersectionWithSphere(const Sphere& sphere,
                                   bool *intersected,
                                   Vector3 *intersectionPoint);

    // Returns the two closest points between the ray and the passed line
    void GetClosestPointToLine(const Vector3 &linePoint,
                               const Vector3 &lineDirection,
                               Vector3 *pointOnRay  = nullptr,
                               Vector3 *pointOnLine = nullptr);

    // Returns the point in the ray that is closer to the passed point
    Vector3 GetClosestPointToPoint(const Vector3 &point);

    Vector3 GetPoint(float t) const;
    const Vector3& GetOrigin() const;
    const Vector3& GetDirection() const;

private:
    Vector3 m_origin = Vector3::Zero;
    Vector3 m_direction = Vector3::Forward;
};

NAMESPACE_BANG_END

#endif // RAY_H

