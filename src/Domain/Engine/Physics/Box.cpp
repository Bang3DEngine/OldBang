#include "Box.h"

#include "Logger.h"

Box::Box()
{
}

Box::Box(float minx, float maxx,
         float miny, float maxy,
         float minz, float maxz) :

    Box(Vector3(minx, miny, minz),
        Vector3(maxx, maxy, maxz))
{
}

Box::Box(const Vector3 &min, const Vector3 &max)
{
    this->minv = min;
    this->maxv = max;
}

Box::Box(const Box &b)
{
    minv = b.minv;
    maxv = b.maxv;
}

Vector3 Box::GetMin() const
{
    return minv;
}

Vector3 Box::GetMax() const
{
    return maxv;
}

Vector3 Box::GetDiagonal() const
{
    return maxv - minv;
}

float Box::GetWidth() const
{
    return (maxv.x - minv.x);
}

float Box::GetHeight() const
{
    return (maxv.y - minv.y);
}

float Box::GetDepth() const
{
    return (maxv.z - minv.z);
}

Vector3 Box::GetCenter() const
{
    return (minv + maxv) / 2.0f;
}

float Box::GetArea() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * 2 +
           w * d * 2 +
           h * d * 2;
}

float Box::GetVolume() const
{
    float w = GetWidth();
    float h = GetHeight();
    float d = GetDepth();
    return w * h * d;
}

Box Box::Union(const Box &b1, const Box &b2)
{
    return
    Box(std::min(b1.minv.x, b2.minv.x), std::max(b1.maxv.x, b2.maxv.x),
        std::min(b1.minv.y, b2.minv.y), std::max(b1.maxv.y, b2.maxv.y),
        std::min(b1.minv.z, b2.minv.z), std::max(b1.maxv.z, b2.maxv.z));
}

void Box::FillFromPositions(const std::vector<Vector3> &positions)
{
    minv = maxv = Vector3::zero;
    for(const Vector3 &v : positions)
    {
        minv.x = std::min(minv.x, v.x);
        maxv.x = std::max(maxv.x, v.x);

        minv.y = std::min(minv.y, v.y);
        maxv.y = std::max(maxv.y, v.y);

        minv.z = std::min(minv.z, v.z);
        maxv.z = std::max(maxv.z, v.z);
    }
}


Box operator*(const Matrix4 &m, const Box &b)
{
    Vector3 minv = b.GetMin();
    Vector3 maxv = b.GetMax();
    glm::vec4 bMinCoords = glm::vec4(minv.x, minv.y, minv.z, 1.0f);
    glm::vec4 bMaxCoords = glm::vec4(maxv.x, maxv.y, maxv.z, 1.0f);
    glm::vec4 v1 = m * bMinCoords;
    glm::vec4 v2 = m * bMaxCoords;

    Box br(std::min(v1.x, v2.x), std::max(v1.x, v2.x),
           std::min(v1.y, v2.y), std::max(v1.y, v2.y),
           std::min(v1.z, v2.z), std::max(v1.z, v2.z));
    return br;
}



const std::string Box::ToString() const
{
    std::ostringstream oss;
    oss << "Box: [" << std::endl <<
           "  min: " << minv << std::endl <<
           "  max" << maxv << std::endl <<
           "]" << std::endl;

    return oss.str();
}
