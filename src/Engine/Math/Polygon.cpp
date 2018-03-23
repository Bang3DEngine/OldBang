#include "Bang/Polygon.h"

#include "Bang/Plane.h"
#include "Bang/Triangle.h"
#include "Bang/Polygon2D.h"

USING_NAMESPACE_BANG

void Polygon::AddPoint(const Vector3 &p)
{
    m_points.PushBack(p);
}

void Polygon::AddPoints(const Array<Vector3> &points)
{
    for (const Vector3 &p : points) { AddPoint(p); }
}

void Polygon::SetPoint(int i, const Vector3 &p)
{
    ASSERT(i >= 0 && i < GetPoints().Size());
    m_points[i] = p;
}

Plane Polygon::GetPlane() const
{
    ASSERT(GetPoints().Size() >= 3);
    return Triangle(GetPoint(0), GetPoint(1), GetPoint(2)).GetPlane();
}

Vector3 Polygon::GetNormal() const
{
    ASSERT(GetPoints().Size() >= 3);
    return Triangle(GetPoint(0), GetPoint(1), GetPoint(2)).GetNormal();
}

Polygon2D Polygon::ProjectedOnAxis(Axis3D axis) const
{
    Polygon2D projectedPoly;
    for (int i = 0; i < GetPoints().Size(); ++i)
    {
        Vector3 p = GetPoint(i);
        Vector2 projP = p.ProjectedOnAxis(axis);
        projectedPoly.AddPoint(projP);
    }
    return projectedPoly;
}

const Vector3 &Polygon::GetPoint(int i) const
{
    ASSERT(i >= 0 && i < GetPoints().Size());
    return GetPoints()[i];
}

const Array<Vector3> &Polygon::GetPoints() const
{
    return m_points;
}

Vector3 &Polygon::operator[](std::size_t i)
{
    return m_points[i];
}

const Vector3 &Polygon::operator[](std::size_t i) const
{
    return GetPoint(i);
}

