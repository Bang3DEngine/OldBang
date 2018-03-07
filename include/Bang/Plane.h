#ifndef PLANE_H
#define PLANE_H

#include "Bang/Bang.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

class Plane
{
public:
    Plane() = default;
    Plane(const Vector3& point, const Vector3 &normal);
    ~Plane() = default;

    void SetPoint(const Vector3& point);
    void SetNormal(const Vector3& normal);

    const Vector3& GetPoint() const;
    const Vector3& GetNormal() const;

private:
    Vector3 m_point = Vector3::Zero;
    Vector3 m_normal = Vector3::Up;
};

NAMESPACE_BANG_END

#endif // PLANE_H

