#ifndef SPHERE_H
#define SPHERE_H

#include "Box.h"
#include "Vector3.h"

class Sphere
{
private:

    Vector3 m_center = Vector3(0.0f);
    float m_radius = 0.0f;

public:

    Sphere();
    Sphere(float m_radius);
    Sphere(Vector3 m_center, float m_radius);

    float GetDiameter() const;
    float GetArea() const;
    float GetVolume() const;

    /**
     * @brief Converts this sphere to a boundingSphere
     * wrapping the box passed as parameter.
     * @param box
     */
    void FillFromBox(const Box &box);

    static Sphere FromBox(const Box &box);

    void SetCenter(const Vector3 &m_center);
    Vector3 GetCenter() const;

    void SetRadius(float m_radius);
    float GetRadius() const;
};

#endif // SPHERE_H
