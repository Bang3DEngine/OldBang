#include "Bang/Rect.h"

#include "Bang/Math.h"
#include "Bang/List.h"
#include "Bang/Camera.h"
#include "Bang/Vector2.h"

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

Rect::Rect(const Vector2 &p1, const Vector2 &p2)
{
    m_minx = Math::Min(p1.x, p2.x);
    m_maxx = Math::Max(p1.x, p2.x);
    m_miny = Math::Min(p1.y, p2.y);
    m_maxy = Math::Max(p1.y, p2.y);
}

void Rect::InvertY()
{
    m_miny *= -1; m_maxy *= -1;
    std::swap(m_miny, m_maxy);
}

Vector2 Rect::GetTopLeft() const
{
    return Vector2(m_minx, m_maxy);
}

Vector2 Rect::GetTopRight() const
{
    return Vector2(m_maxx, m_maxy);
}

Vector2 Rect::GetBotLeft() const
{
    return Vector2(m_minx, m_miny);
}

Vector2 Rect::GetBotRight() const
{
    return Vector2(m_maxx, m_miny);
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

Vector2 Rect::GetSize() const
{
    return Vector2(GetWidth(), GetHeight());
}

Vector2 Rect::GetCenter() const
{
    return (GetMin() + GetMax()) * 0.5f;
}

bool Rect::Contains(const Vector2 &p) const
{
    return p.x >= m_minx && p.x <= m_maxx &&
           p.y >= m_miny && p.y <= m_maxy;
}

Rect Rect::Union(const Rect &r1, const Rect &r2)
{
    if (r1 == Rect::Empty) { return r2; }
    if (r2 == Rect::Empty) { return r1; }
    return Rect(Math::Min(r1.m_minx, r2.m_minx),
                Math::Max(r1.m_maxx, r2.m_maxx),
                Math::Min(r1.m_miny, r2.m_miny),
                Math::Max(r1.m_maxy, r2.m_maxy));
}

Rect Rect::Intersection(const Rect &r1, const Rect &r2)
{
    float minx = Math::Max(r1.m_minx, r2.m_minx);
    float miny = Math::Max(r1.m_miny, r2.m_miny);
    float maxx = Math::Min(r1.m_maxx, r2.m_maxx);
    float maxy = Math::Min(r1.m_maxy, r2.m_maxy);

    if (minx > maxx || miny > maxy)
    {
        return Rect::Empty;
    }

    return Rect(minx, maxx, miny, maxy);
}

Rect Rect::GetBoundingRectFromPositions(const List<Vector2> &positions)
{
    if (positions.Empty()) { return Rect::Empty; }

    Vector2 minv = positions.Front(), maxv = positions.Front();
    for (const Vector2 &p : positions)
    {
        minv.x = Math::Min(p.x, minv.x);
        minv.y = Math::Min(p.y, minv.y);
        maxv.x = Math::Max(p.x, maxv.x);
        maxv.y = Math::Max(p.y, maxv.y);
    }

    return Rect(minv, maxv);
}

String Rect::ToString() const
{
    std::ostringstream oss;
    oss << "Rect(" << GetMin().ToString() << ", " << GetMax().ToString() << ")";
    return String(oss.str());
}


bool operator==(const Rect &r1, const Rect &r2)
{
    return r1.GetMin() == r2.GetMin() &&
           r1.GetMax() == r2.GetMax();
}


bool operator!=(const Rect &r1, const Rect &r2)
{
    return !(r1 == r2);
}

void operator*=(Rect &r, float a)
{
    r.m_minx *= a;
    r.m_miny *= a;
    r.m_maxx *= a;
    r.m_maxy *= a;
}

void operator/=(Rect &r, float a)
{
    r.m_minx /= a;
    r.m_miny /= a;
    r.m_maxx /= a;
    r.m_maxy /= a;
}

void operator*=(Rect &r, const Vector2 &v)
{
    r.m_minx *= v.x;
    r.m_miny *= v.y;
    r.m_maxx *= v.x;
    r.m_maxy *= v.y;
}

void operator/=(Rect &r, const Vector2 &v)
{
    r.m_minx /= v.x;
    r.m_miny /= v.y;
    r.m_maxx /= v.x;
    r.m_maxy /= v.y;
}

Rect operator*(const Matrix4 &m, const Rect &r)
{
    return Rect( (m * Vector4(r.GetMin(),0,1) ).xy(),
                 (m * Vector4(r.GetMax(),0,1) ).xy() );
}
