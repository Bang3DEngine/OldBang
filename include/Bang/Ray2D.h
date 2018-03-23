#ifndef RAY2D_H
#define RAY2D_H

#include "Bang/Bang.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

class Ray2D
{
public:
    Ray2D();
    Ray2D(const Vector2 &origin, const Vector2 &direction);
    ~Ray2D();

    void SetOrigin(const Vector2 &origin);
    void SetDirection(const Vector2 &direction);

    // Gets a point along the ray with distance t to the ray origin
    Vector2 GetPoint(float t) const;

    const Vector2& GetOrigin() const;
    const Vector2& GetDirection() const;

private:
    Vector2 m_origin = Vector2::Zero;
    Vector2 m_direction = Vector2(1,0);
};

NAMESPACE_BANG_END

#endif // RAY2D_H

