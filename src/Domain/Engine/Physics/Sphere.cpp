#include "Sphere.h"

Sphere::Sphere()
{

}

Sphere::Sphere(float radius) :
    m_radius(radius)
{
}

Sphere::Sphere(Vector3 center, float radius) :
    m_center(center),
    m_radius(radius)
{

}

float Sphere::GetDiameter() const
{
    return 2 * m_radius;
}

float Sphere::GetArea() const
{
    return 2 * glm::pi<float>() * m_radius;
}

float Sphere::GetVolume() const
{
    return glm::pi<float>() * m_radius * m_radius;
}

void Sphere::FillFromBox(const Box &box)
{
    m_radius = box.GetDiagonal().Length() / 2.0f;
    m_center = box.GetCenter();
}

Sphere Sphere::FromBox(const Box &box)
{
    Sphere s;
    s.FillFromBox(box);
    return s;
}

void Sphere::SetCenter(const Vector3 &center)
{
    this->m_center = center;
}

Vector3 Sphere::GetCenter() const
{
    return m_center;
}

void Sphere::SetRadius(float radius)
{
    this->m_radius = radius;
}

float Sphere::GetRadius() const
{
    return m_radius;
}
