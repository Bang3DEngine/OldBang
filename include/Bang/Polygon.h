#ifndef POLYGON_H
#define POLYGON_H

#include "Bang/Bang.h"
#include "Bang/Axis.h"
#include "Bang/Array.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

FORWARD class Plane;
FORWARD class Polygon2D;

class Polygon
{
public:
    Polygon() = default;
    ~Polygon() = default;

    void AddPoint(const Vector3 &p);
    void AddPoints(const Array<Vector3> &points);
    void SetPoint(int i, const Vector3 &p);

    Plane GetPlane() const;
    Vector3 GetNormal() const;
    Polygon2D ProjectedOnAxis(Axis3D axis) const;

    const Vector3& GetPoint(int i) const;
    const Array<Vector3>& GetPoints() const;

    Vector3& operator[](std::size_t i);
    const Vector3& operator[](std::size_t i) const;

private:
    Array<Vector3> m_points;
};

NAMESPACE_BANG_END

#endif // POLYGON_H

