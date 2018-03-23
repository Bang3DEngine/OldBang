#ifndef SEGMENT2D_H
#define SEGMENT2D_H

#include "Bang/Bang.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

class Segment2D
{
public:
    Segment2D() = default;
    Segment2D(const Vector2 &origin, const Vector2 &destiny);
    ~Segment2D() = default;

    void SetOrigin(const Vector2& p);
    void SetDestiny(const Vector2& p);

    float GetLength() const;
    Vector2 GetDirection() const;
    const Vector2& GetOrigin() const;
    const Vector2& GetDestiny() const;

private:
    Vector2 m_origin  = Vector2::Zero;
    Vector2 m_destiny = Vector2::Zero;
};

NAMESPACE_BANG_END

#endif // SEGMENT2D_H

