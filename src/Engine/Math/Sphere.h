#ifndef SPHERE_H
#define SPHERE_H

#include "Bang/AABox.h"
#include "Bang/Vector3.h"

class Sphere
{
public:
    Sphere();
    Sphere(float m_radius);
    Sphere(const Vector3 &m_center, float m_radius);

    void SetCenter(const Vector3 &m_center);
    void SetRadius(float m_radius);

    Array<Vector3> GetPoints() const;
    float GetDiameter() const;
    float GetArea() const;
    float GetVolume() const;
    const Vector3 & GetCenter() const;
    float GetRadius() const;

    bool Contains(const Vector3 &point) const;
    bool CheckCollision(const Sphere &sphere) const;
    bool CheckCollision(const AABox &aabox,
                        Vector3 *point  = nullptr,
                        Vector3 *normal = nullptr) const;

    void FillFromBox(const AABox &box);

    static Sphere FromBox(const AABox &box);

private:
    Vector3 m_center = Vector3(0.0f);
    float m_radius = 0.0f;
};

#endif // SPHERE_H
