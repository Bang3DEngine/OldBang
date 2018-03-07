#include "Bang/AABox.h"

#include "Bang/List.h"
#include "Bang/Math.h"
#include "Bang/Array.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/Sphere.h"
#include "Bang/Vector2.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

NAMESPACE_BANG_BEGIN

AABox AABox::Empty = AABox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

AABox::AABox(float minx, float maxx,
             float miny, float maxy,
             float minz, float maxz) :
    AABox(Vector3(minx, miny, minz),
          Vector3(maxx, maxy, maxz))
{
}

AABox::AABox(const Vector3 &p1, const Vector3 &p2)
{
    m_minv = Vector3::Min(p1, p2);
    m_maxv = Vector3::Max(p1, p2);
}

AABox::AABox(const AABox &b)
{
    m_minv = b.m_minv;
    m_maxv = b.m_maxv;
}

void AABox::SetMin(const Vector3 &bMin)
{
    m_minv = bMin;
}

void AABox::SetMax(const Vector3 &bMax)
{
    m_maxv = bMax;
}

const Vector3& AABox::GetMin() const
{
    return m_minv;
}

const Vector3& AABox::GetMax() const
{
    return m_maxv;
}

Vector3 AABox::GetDiagonal() const
{
    return m_maxv - m_minv;
}

float AABox::GetWidth() const
{
    return (m_maxv.x - m_minv.x);
}

float AABox::GetHeight() const
{
    return (m_maxv.y - m_minv.y);
}

float AABox::GetDepth() const
{
    return (m_maxv.z - m_minv.z);
}

Vector3 AABox::GetCenter() const
{
    return (m_minv + m_maxv) / 2.0f;
}

Vector3 AABox::GetDimensions() const
{
    return Vector3(GetWidth(), GetHeight(), GetDepth());
}

float AABox::GetArea() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * 2 +
           w * d * 2 +
           h * d * 2;
}

float AABox::GetVolume() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * d;
}

Vector3 AABox::GetExtents() const
{
    return (GetMax() - GetMin()) / 2.0f;
}

Vector3 AABox::GetClosestPointInAABB(const Vector3 &point) const
{
    Vector3 closestPoint;
    closestPoint.x = Math::Clamp(point.x, m_minv.x, m_maxv.x);
    closestPoint.y = Math::Clamp(point.y, m_minv.y, m_maxv.y);
    closestPoint.z = Math::Clamp(point.z, m_minv.z, m_maxv.z);
    return closestPoint;
}

bool AABox::CheckCollision(const Sphere &sphere,
                           Vector3 *point,
                           Vector3 *normal) const
{
    Vector3 closestPointToAABox = GetClosestPointInAABB(sphere.GetCenter());

    float sRadius = sphere.GetRadius();
    const Vector3 &sCenter = sphere.GetCenter();
    float dCenterClosest = Vector3::Distance(closestPointToAABox, sCenter);
    bool collides = ( dCenterClosest <= sRadius * sRadius );
    if (collides)
    {
        if (point)  { *point = closestPointToAABox; }
        if (normal) { *normal = (closestPointToAABox - sCenter).Normalized(); }
    }
    return collides;
}

bool AABox::CheckCollision(const AABox &aabox) const
{
    Array<Vector3> points = aabox.GetPoints();
    return Contains(points[0]) || Contains(points[1]) || Contains(points[2]) ||
           Contains(points[3]) || Contains(points[4]) || Contains(points[5]) ||
           Contains(points[6]) || Contains(points[7]);

}

bool AABox::Contains(const Vector3 &point) const
{
    return point.x >= m_minv.x && point.x <= m_maxv.x &&
           point.y >= m_minv.y && point.y <= m_maxv.y &&
           point.z >= m_minv.z && point.z <= m_maxv.z;
}

void AABox::AddPoint(const Vector3 &point)
{
    SetMin( Vector3::Min(GetMin(), point) );
    SetMax( Vector3::Max(GetMax(), point) );
}

AABox AABox::Union(const AABox &b1, const AABox &b2)
{
    if (b1 == AABox::Empty) { return b2; }
    if (b2 == AABox::Empty) { return b1; }
    return AABox(Vector3::Min(b1.GetMin(), b2.GetMin()),
                 Vector3::Max(b1.GetMax(), b2.GetMax()));
}

void AABox::FillFromPositions(const Array<Vector3> &positions)
{
    if (positions.IsEmpty()) { return; }
    m_minv = m_maxv = positions[0];
    for (const Vector3 &v : positions) { AddPoint(v); }
}

AABox AABox::FromSphere(const Sphere &sphere)
{
    AABox b;
    b.FillFromPositions(sphere.GetPoints());
    return b;
}

Array<Vector3> AABox::GetPoints() const
{
    const Vector3 center  = GetCenter();
    const Vector3 extents = GetExtents();
    const Vector3 p1 = center + extents * Vector3(-1, -1, -1);
    const Vector3 p2 = center + extents * Vector3(-1, -1,  1);
    const Vector3 p3 = center + extents * Vector3(-1,  1, -1);
    const Vector3 p4 = center + extents * Vector3(-1,  1,  1);
    const Vector3 p5 = center + extents * Vector3( 1, -1, -1);
    const Vector3 p6 = center + extents * Vector3( 1, -1,  1);
    const Vector3 p7 = center + extents * Vector3( 1,  1, -1);
    const Vector3 p8 = center + extents * Vector3( 1,  1,  1);
    return {p1, p2, p3, p4, p5, p6, p7, p8};
}

AARect AABox::GetAABoundingViewportRect(Camera *cam) const
{
    Vector3 camPosition = cam->GetGameObject()->GetTransform()->GetPosition();
    if ( Contains(camPosition) ) { return AARect::NDCRect; }
    Array<Vector3> boxPoints = (*this).GetPoints();

    List<Vector2> viewportPoints;
    bool somePointInFront = false;
    Vector3 camForward = cam->GetGameObject()->GetTransform()->GetForward();
    for (const Vector3 &p : boxPoints)
    {
        somePointInFront = somePointInFront ||
                           Vector3::Dot(p-camPosition, camForward) > 0;

        Vector2 viewportP = cam->FromWorldPointToViewportPointNDC(p);
        viewportPoints.PushBack(viewportP);
    }
    if (!somePointInFront) { return AARect::Zero; }

    AARect boundingRect = AARect::GetBoundingRectFromPositions(viewportPoints.Begin(),
                                                           viewportPoints.End());
    return boundingRect;
}

AABox operator*(const Matrix4 &m, const AABox &b)
{
    Array<Vector3> points = b.GetPoints();
    Array<Vector3> newTransformedBoxPoints =
        {
            (m * Vector4(points[0], 1)).xyz(),
            (m * Vector4(points[1], 1)).xyz(),
            (m * Vector4(points[2], 1)).xyz(),
            (m * Vector4(points[3], 1)).xyz(),
            (m * Vector4(points[4], 1)).xyz(),
            (m * Vector4(points[5], 1)).xyz(),
            (m * Vector4(points[6], 1)).xyz(),
            (m * Vector4(points[7], 1)).xyz()
        };

    AABox br;
    br.FillFromPositions(newTransformedBoxPoints);
    return br;
}



String AABox::ToString() const
{
    std::ostringstream oss;
    oss << "Box(" << m_minv << ", " << m_maxv << ")" << std::endl;
    return oss.str();
}


bool operator==(const AABox &b1, const AABox &b2)
{
    return (b1.GetMin() == b2.GetMin()) && (b1.GetMax() == b2.GetMax());
}

bool operator!=(const AABox &b1, const AABox &b2)
{
    return !(b1 == b2);
}

NAMESPACE_BANG_END
