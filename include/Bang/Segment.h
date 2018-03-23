#ifndef SEGMENT_H
#define SEGMENT_H

#include "Bang/Bang.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

class Segment
{
public:
    Segment() = default;
    Segment(const Vector3 &origin, const Vector3 &destiny);
    ~Segment() = default;

    void SetOrigin(const Vector3& p);
    void SetDestiny(const Vector3& p);

    float GetLength() const;
    Vector3 GetDirection() const;
    const Vector3& GetOrigin() const;
    const Vector3& GetDestiny() const;

private:
    Vector3 m_origin  = Vector3::Zero;
    Vector3 m_destiny = Vector3::Zero;
};

NAMESPACE_BANG_END

#endif // SEGMENT_H

