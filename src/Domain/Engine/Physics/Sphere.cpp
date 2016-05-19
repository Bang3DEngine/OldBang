#include "Sphere.h"

const float Sphere::PI = 3.1415926f;

Sphere::Sphere()
{

}

Sphere::Sphere(float radius) :
    radius(radius)
{
}

Sphere::Sphere(Vector3 center, float radius) :
    center(center),
    radius(radius)
{

}

float Sphere::GetDiameter() const
{
    return 2 * radius;
}

float Sphere::GetArea() const
{
    return 2 * PI * radius;
}

float Sphere::GetVolume() const
{
    return PI * radius * radius;
}

void Sphere::FillFromBox(const Box &box)
{
    radius = box.GetDiagonal().Length();
    center = box.GetCenter();
}

Sphere Sphere::FromBox(const Box &box)
{
    Sphere s;
    s.FillFromBox(box);
    return s;
}

void Sphere::SetCenter(const Vector3 &center)
{
    this->center = center;
}

Vector3 Sphere::GetCenter() const
{
    return center;
}

void Sphere::SetRadius(float radius)
{
    this->radius = radius;
}

float Sphere::GetRadius() const
{
    return radius;
}
