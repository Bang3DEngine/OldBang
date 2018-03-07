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

    // Gets a point along the ray with distance t to the ray origin
    Vector3 GetPoint(float t) const;

    const Vector3& GetOrigin() const;
    const Vector3& GetDirection() const;

private:
    Vector3 m_origin = Vector3::Zero;
    Vector3 m_direction = Vector3::Forward;
};

NAMESPACE_BANG_END

#endif // RAY_H

