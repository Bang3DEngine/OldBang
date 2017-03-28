#ifndef SPHERE_H
#define SPHERE_H

#include "Bang/Array.h"
#include "Bang/AABox.h"
#include "Bang/Vector3.h"

class Sphere
{
private:

    Vector3 m_center = Vector3(0.0f);
    float m_radius = 0.0f;

public:

    Sphere();
    Sphere(float m_radius);
    Sphere(const Vector3 &m_center, float m_radius);

    Array<Vector3> GetPoints() const;

    float GetDiameter() const;
    float GetArea() const;
    float GetVolume() const;

    bool Contains(const Vector3 &point) const;
    bool CheckCollision(const Sphere &sphere) const;
    bool CheckCollision(const AABox &aabox,
                        Vector3 *point,
                        Vector3 *normal) const;

    /**
     * @brief Converts this sphere to a boundingSphere
     * wrapping the box passed as parameter.
     * @param box
     */
    void FillFromBox(const AABox &box);

    static Sphere FromBox(const AABox &box);

    void SetCenter(const Vector3 &m_center);
    const Vector3 & GetCenter() const;

    void SetRadius(float m_radius);
    float GetRadius() const;
};

#endif // SPHERE_H
