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

AABox AABox::Empty = AABox();

AABox::AABox()
{
}

AABox::AABox(float minx, float maxx,
             float miny, float maxy,
             float minz, float maxz) :
    AABox(Vector3(minx, miny, minz),
          Vector3(maxx, maxy, maxz))
{
}

AABox::AABox(const Vector3 &p)
{
    SetMin(p);
    SetMax(p);
}

AABox::AABox(const Vector3 &p1, const Vector3 &p2)
{
    SetMin(Vector3::Min(p1, p2));
    SetMax(Vector3::Max(p1, p2));
}

AABox::AABox(const AABox &b)
{
    // Dont use setters to avoid initialized check in case b is not initialized
    SetMin(b.m_minv);
    SetMax(b.m_maxv);
}

void AABox::SetMin(const Vector3 &bMin)
{
    m_minv = bMin;
    m_initialized = true;
}

void AABox::SetMax(const Vector3 &bMax)
{
    m_maxv = bMax;
    m_initialized = true;
}

const Vector3& AABox::GetMin() const
{
    ASSERT(m_initialized);
    return m_minv;
}

const Vector3& AABox::GetMax() const
{
    ASSERT(m_initialized);
    return m_maxv;
}

Vector3 AABox::GetDiagonal() const
{
    return GetMax() - GetMin();
}

float AABox::GetWidth() const
{
    return (GetMax().x - GetMin().x);
}

float AABox::GetHeight() const
{
    return (GetMax().y - GetMin().y);
}

float AABox::GetDepth() const
{
    return (GetMax().z - GetMin().z);
}

Vector3 AABox::GetCenter() const
{
    return (GetMin() + GetMax()) / 2.0f;
}

Vector3 AABox::GetSize() const
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
    closestPoint.x = Math::Clamp(point.x, GetMin().x, GetMax().x);
    closestPoint.y = Math::Clamp(point.y, GetMin().y, GetMax().y);
    closestPoint.z = Math::Clamp(point.z, GetMin().z, GetMax().z);
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
    if (!m_initialized) { return false; }
    // return point.x >= GetMin().x && point.x <= GetMax().x &&
    //        point.y >= GetMin().y && point.y <= GetMax().y &&
    //        point.z >= GetMin().z && point.z <= GetMax().z;

    return point >= GetMin() && point <= GetMax();
}

void AABox::AddPoint(const Vector3 &point)
{
    if (!m_initialized) { SetMin(point); SetMax(point); }

    SetMin( Vector3::Min(GetMin(), point) );
    SetMax( Vector3::Max(GetMax(), point) );
}

AABox AABox::Union(const AABox &b1, const AABox &b2)
{
    if (!b1.m_initialized && !b2.m_initialized) { return AABox::Empty; }
    if (!b1.m_initialized) { return b2; }
    if (!b2.m_initialized) { return b1; }
    return AABox(Vector3::Min(b1.GetMin(), b2.GetMin()),
                 Vector3::Max(b1.GetMax(), b2.GetMax()));
}

void AABox::CreateFromPositions(const Array<Vector3> &positions)
{
    if (positions.IsEmpty()) { return; }
    for (const Vector3 &v : positions) { AddPoint(v); }
}

AABox AABox::FromPointAndSize(const Vector3 &point, const Vector3 &size)
{
    return AABox(point, point + size);
}

AABox AABox::FromSphere(const Sphere &sphere)
{
    AABox b (sphere.GetPoints().Front());
    b.CreateFromPositions(sphere.GetPoints());
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

Quad AABox::GetQuad(Axis3D axis, bool sign) const
{
    const Vector3 &ctr = GetCenter();
    const Vector3 &ext = GetExtents();
    int xs0, xs1, xs2, xs3, ys0, ys1, ys2, ys3, zs0, zs1, zs2, zs3;
    switch (axis) // Given in CCW order out facing the AABox
    {
        case Axis3D::X:
            xs0 = (sign ?  1 : -1); xs1 = (sign ?  1 : -1);
            xs2 = (sign ?  1 : -1); xs3 = (sign ?  1 : -1);
            ys0 = (sign ? -1 : -1); ys1 = (sign ? -1 : -1);
            ys2 = (sign ?  1 :  1); ys3 = (sign ?  1 :  1);
            zs0 = (sign ?  1 : -1); zs1 = (sign ? -1 :  1);
            zs2 = (sign ? -1 :  1); zs3 = (sign ?  1 : -1);
        break;

        case Axis3D::Y:
            xs0 = (sign ? -1 : -1); xs1 = (sign ?  1 : -1);
            xs2 = (sign ?  1 :  1); xs3 = (sign ? -1 :  1);
            ys0 = (sign ?  1 : -1); ys1 = (sign ?  1 : -1);
            ys2 = (sign ?  1 : -1); ys3 = (sign ?  1 : -1);
            zs0 = (sign ?  1 :  1); zs1 = (sign ?  1 : -1);
            zs2 = (sign ? -1 : -1); zs3 = (sign ? -1 :  1);
        break;

        default:
        case Axis3D::Z:
            xs0 = (sign ? -1 : -1); xs1 = (sign ?  1 : -1);
            xs2 = (sign ?  1 :  1); xs3 = (sign ? -1 :  1);
            ys0 = (sign ? -1 : -1); ys1 = (sign ? -1 :  1);
            ys2 = (sign ?  1 :  1); ys3 = (sign ?  1 : -1);
            zs0 = (sign ?  1 : -1); zs1 = (sign ?  1 : -1);
            zs2 = (sign ?  1 : -1); zs3 = (sign ?  1 : -1);
        break;

    }
    return Quad(ctr + Vector3(xs0 * ext.x, ys0 * ext.y, zs0 * ext.z),
                ctr + Vector3(xs1 * ext.x, ys1 * ext.y, zs1 * ext.z),
                ctr + Vector3(xs2 * ext.x, ys2 * ext.y, zs2 * ext.z),
                ctr + Vector3(xs3 * ext.x, ys3 * ext.y, zs3 * ext.z));
}
Quad AABox::GetRightQuad() const { return GetQuad(Axis3D::X, true); }
Quad AABox::GetLeftQuad()  const { return GetQuad(Axis3D::X, false); }
Quad AABox::GetTopQuad()   const { return GetQuad(Axis3D::Y, true); }
Quad AABox::GetBotQuad()   const { return GetQuad(Axis3D::Y, false); }
Quad AABox::GetFrontQuad() const { return GetQuad(Axis3D::Z, true); }
Quad AABox::GetBackQuad()  const { return GetQuad(Axis3D::Z, false); }
std::array<Quad, 6> AABox::GetQuads() const
{
    const std::array<Quad, 6> aaBoxQuads = {GetBotQuad(),   GetTopQuad(),
                                            GetLeftQuad(),  GetRightQuad(),
                                            GetFrontQuad(), GetBackQuad()};
    return aaBoxQuads;
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

    AABox br (newTransformedBoxPoints.Front());
    br.CreateFromPositions(newTransformedBoxPoints);
    return br;
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
