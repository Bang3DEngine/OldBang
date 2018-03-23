#include "Bang/Polygon2D.h"

#include "Bang/Ray2D.h"
#include "Bang/Geometry.h"
#include "Bang/Segment2D.h"

USING_NAMESPACE_BANG

Polygon2D::Polygon2D()
{
}

Polygon2D::~Polygon2D()
{
}

void Polygon2D::AddPoint(const Vector2 &p)
{
    m_points.PushBack(p);
}

void Polygon2D::SetPoint(int i, const Vector2 &p)
{
    ASSERT(i >= 0 && i < GetPoints().Size());
    m_points[i] = p;
}

bool Polygon2D::Contains(const Vector2 &p)
{
    ASSERT(GetPoints().Size() >= 3);

    Vector2 minPoint = GetPoint(0);
    Vector2 maxPoint = GetPoint(0);
    for (const Vector2 &point : GetPoints())
    {
        minPoint = Vector2::Min(minPoint, point);
        maxPoint = Vector2::Max(maxPoint, point);
    }
    float VeryFar = Vector2::Distance(minPoint, maxPoint) * 10;

    int intersectionCount = 0;
    Ray2D testRay (p, p + Vector2(VeryFar));
    for (int i = 0; i < GetPoints().Size(); ++i)
    {
        const Segment2D segment(GetPoint(i),
                                GetPoint( (i+1) % GetPoints().Size() ) );

        bool intersected;
        Vector2 intersPoint;
        Geometry::IntersectRay2DSegment2D(testRay, segment,
                                          &intersected, &intersPoint);
        intersectionCount += (intersected ? 1 : 0);
    }
    return (intersectionCount % 2) == 1;
}

const Vector2 &Polygon2D::GetPoint(int i) const
{
    ASSERT(i >= 0 && i < GetPoints().Size());
    return m_points[i];
}

const Array<Vector2> &Polygon2D::GetPoints() const
{
    return m_points;
}

