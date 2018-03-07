#include "Bang/Triangle.h"

USING_NAMESPACE_BANG

Triangle::Triangle(const Vector3 &point0,
                   const Vector3 &point1,
                   const Vector3 &point2)
{
    m_points[0] = point0;
    m_points[1] = point1;
    m_points[2] = point2;
}

void Triangle::SetPoint(int i, const Vector3 &point)
{
    ASSERT(i >= 0 && i <= 3);
    m_points[i] = point;
}

const Vector3 &Triangle::GetPoint(int i) const
{
    ASSERT(i >= 0 && i <= 2);
    return GetPoints()[i];
}

const std::array<Vector3, 3> &Triangle::GetPoints() const
{
    return m_points;
}

