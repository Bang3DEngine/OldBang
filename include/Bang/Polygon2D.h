#ifndef POLYGON2D_H
#define POLYGON2D_H

#include "Bang/Bang.h"
#include "Bang/Array.h"
#include "Bang/Vector2.h"

NAMESPACE_BANG_BEGIN

class Polygon2D
{
public:
	Polygon2D();
    ~Polygon2D();

    void AddPoint(const Vector2 &p);
    void SetPoint(int i, const Vector2 &p);
    bool Contains(const Vector2 &p);

    const Vector2& GetPoint(int i) const;
    const Array<Vector2>& GetPoints() const;

private:
    Array<Vector2> m_points;
};

NAMESPACE_BANG_END

#endif // POLYGON2D_H

