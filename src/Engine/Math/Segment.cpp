#include "Bang/Segment.h"

USING_NAMESPACE_BANG

Segment::Segment(const Vector3 &origin, const Vector3 &destiny)
{
    SetOrigin(origin);
    SetDestiny(destiny);
}

void Segment::SetOrigin(const Vector3 &p)
{
    m_origin = p;
}

void Segment::SetDestiny(const Vector3 &p)
{
    m_destiny = p;
}

float Segment::GetLength() const
{
    return (GetDestiny() - GetOrigin()).Length();
}

Vector3 Segment::GetDirection() const
{
    return (GetDestiny() - GetOrigin()).NormalizedSafe();
}

const Vector3 &Segment::GetOrigin() const
{
    return m_origin;
}

const Vector3 &Segment::GetDestiny() const
{
    return m_destiny;
}

