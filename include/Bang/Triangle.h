#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <array>

#include "Bang/Bang.h"
#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

class Triangle
{
public:
    Triangle() = default;
    Triangle(const Vector3 &point0,
             const Vector3 &point1,
             const Vector3 &point2);
    ~Triangle() = default;

    void SetPoint(int i, const Vector3 &point);

    Vector3 GetNormal() const;
    const std::array<Vector3, 3>& GetPoints() const;
    const Vector3& GetPoint(int i) const;

    Vector3& operator[](std::size_t i);
    const Vector3& operator[](std::size_t i) const;

private:
    std::array<Vector3, 3> m_points;
};

Triangle operator*(const Matrix4 &m, const Triangle &t);

NAMESPACE_BANG_END

#endif // TRIANGLE_H

