#ifndef QUAD_H
#define QUAD_H

#include <array>

#include "Bang/Bang.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

FORWARD class Triangle;

class Quad
{
public:
    Quad() = default;

    // Points must be in CCW order!
    Quad(const Vector3 &p0,
         const Vector3 &p1,
         const Vector3 &p2,
         const Vector3 &p3);

    ~Quad() = default;

    void SetPoint(int i, const Vector3 &p);

    Vector3 GetNormal() const;
    const Vector3& GetPoint(int i) const;
    const std::array<Vector3, 4>& GetPoints() const;

    // Returns the two triangles of this quad
    void GetTriangles(Triangle *t0, Triangle *t1) const;

    Vector3& operator[](std::size_t i);
    const Vector3& operator[](std::size_t i) const;

private:
    std::array<Vector3, 4> m_points = {Vector3::Zero,
                                       Vector3::Zero,
                                       Vector3::Zero,
                                       Vector3::Zero};
};

NAMESPACE_BANG_END

#endif // QUAD_H

