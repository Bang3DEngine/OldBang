#include "Bang/Triangle.h"

#include "Bang/Plane.h"
#include "Bang/Matrix4.h"
#include "Bang/Polygon.h"

USING_NAMESPACE_BANG

Triangle::Triangle(const Vector3 &point0,
                   const Vector3 &point1,
                   const Vector3 &point2)
{
    SetPoint(0, point0);
    SetPoint(1, point1);
    SetPoint(2, point2);
}

void Triangle::SetPoint(int i, const Vector3 &point)
{
    (*this)[i] = point;
}

Plane Triangle::GetPlane() const
{
    return Plane(GetPoint(0), GetNormal());
}

Vector3 Triangle::GetNormal() const
{
    return Vector3::Cross(GetPoint(1)-GetPoint(0), GetPoint(2)-GetPoint(0)).
                    NormalizedSafe();
}

const Vector3 &Triangle::GetPoint(int i) const
{
    return GetPoints()[i];
}

Polygon Triangle::ToPolygon() const
{
    Polygon poly;
    poly.AddPoint( GetPoint(0) );
    poly.AddPoint( GetPoint(1) );
    poly.AddPoint( GetPoint(2) );
    return poly;
}

const std::array<Vector3, 3> &Triangle::GetPoints() const
{
    return m_points;
}

Vector3 &Triangle::operator[](std::size_t i)
{
    ASSERT(i >= 0 && i <= 2);
    return m_points[i];
}

const Vector3 &Triangle::operator[](std::size_t i) const
{
    ASSERT(i >= 0 && i <= 2);
    return m_points[i];
}

NAMESPACE_BANG_BEGIN

Triangle operator*(const Matrix4 &m, const Triangle &t)
{
    return Triangle(m.TransformPoint(t[0]),
                    m.TransformPoint(t[1]),
                    m.TransformPoint(t[2]));
}

NAMESPACE_BANG_END
