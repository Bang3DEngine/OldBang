#include "Rect.h"

#include "List.h"
#include "Vector2.h"

Rect Rect::ScreenRect = Rect(Vector2(-1), Vector2(1));
Rect Rect::Empty = Rect(0, 0, 0, 0);

Rect::Rect()
{
}

Rect::Rect(float minx, float maxx,
           float miny, float maxy) :
    m_minx(minx), m_maxx(maxx),
    m_miny(miny), m_maxy(maxy)
{
}

Rect::Rect(Vector2 p1, Vector2 p2)
{
    m_minx = glm::min(p1.x, p2.x);
    m_maxx = glm::max(p1.x, p2.x);
    m_miny = glm::min(p1.y, p2.y);
    m_maxy = glm::max(p1.y, p2.y);
}

Vector2 Rect::GetMin() const
{
    return Vector2(m_minx, m_miny);
}

Vector2 Rect::GetMax() const
{
    return Vector2(m_maxx, m_maxy);
}

float Rect::GetWidth() const
{
    return (m_maxx-m_minx);
}

float Rect::GetHeight() const
{
    return (m_maxy-m_miny);
}

float Rect::GetArea() const
{
    return (m_maxx-m_minx) * (m_maxy-m_miny);
}

bool Rect::Contains(const Vector2 &p) const
{
    return p.x >= m_minx && p.x <= m_maxx &&
           p.y >= m_miny && p.y <= m_maxy;
}

Rect Rect::Union(const Rect &r1, const Rect &r2)
{
    return Rect(glm::min(r1.m_minx, r2.m_minx),
                glm::min(r1.m_maxx, r2.m_maxx),
                glm::max(r1.m_miny, r2.m_miny),
                glm::max(r1.m_maxy, r2.m_maxy));
}

Rect Rect::Intersection(const Rect &r1, const Rect &r2)
{
    float minx = glm::max(r1.m_minx, r2.m_minx);
    float miny = glm::max(r1.m_miny, r2.m_miny);
    float maxx = glm::min(r1.m_maxx, r2.m_maxx);
    float maxy = glm::min(r1.m_maxy, r2.m_maxy);

    if (minx > maxx || miny > maxy)
    {
        return Rect::Empty;
    }

    return Rect(minx, maxx, miny, maxy);
}

Rect Rect::GetBoundingRectFromPositions(const List<Vector2> &positions)
{
    Vector2 minv = positions.Front(), maxv = positions.Front();
    for (const Vector2 &p : positions)
    {
        minv.x = glm::min(p.x, minv.x);
        minv.y = glm::min(p.y, minv.y);
        maxv.x = glm::max(p.x, maxv.x);
        maxv.y = glm::max(p.y, maxv.y);
    }

    return Rect(minv, maxv);
}

const String Rect::ToString() const
{
    std::ostringstream oss;
    oss << "Rect: [" << std::endl <<
           "  minx:" << m_minx << std::endl <<
           "  maxx:" << m_maxx << std::endl <<
           "  miny:" << m_miny << std::endl <<
           "  maxy:" << m_maxy << std::endl <<
           "]" << std::endl;

    return String(oss.str());
}
