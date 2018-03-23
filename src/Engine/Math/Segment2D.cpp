#include "Bang/Segment2D.h"

USING_NAMESPACE_BANG

Segment2D::Segment2D(const Vector2 &origin, const Vector2 &destiny)
{
    SetOrigin(origin);
    SetDestiny(destiny);
}

void Segment2D::SetOrigin(const Vector2 &p)
{
    m_origin = p;
}

void Segment2D::SetDestiny(const Vector2 &p)
{
    m_destiny = p;
}

float Segment2D::GetLength() const
{
    return (GetDestiny() - GetOrigin()).Length();
}

Vector2 Segment2D::GetDirection() const
{
    return (GetDestiny() - GetOrigin()).NormalizedSafe();
}

const Vector2 &Segment2D::GetOrigin() const
{
    return m_origin;
}

const Vector2 &Segment2D::GetDestiny() const
{
    return m_destiny;
}

