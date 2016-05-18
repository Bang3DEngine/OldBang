#include "Box.h"

#include "Logger.h"

Box::Box()
{
}

const std::string Box::ToString() const
{
    std::ostringstream oss;
    oss << "Box: [" << std::endl <<
           "  x(" << minx << ", " << maxx << ")" << std::endl <<
           "  y(" << miny << ", " << maxy << ")" << std::endl <<
           "  z(" << minz << ", " << maxz << ")" << std::endl <<
           "]" << std::endl;

    return oss.str();
}

Box::Box(float minx, float maxx,
         float miny, float maxy,
         float minz, float maxz) :
    minx(minx), maxx(maxx),
    miny(miny), maxy(maxy),
    minz(minz), maxz(maxz)
{
}

Box::Box(const Box &b)
{
    minx = b.minx; maxx = b.maxx;
    miny = b.miny; maxy = b.maxy;
    minz = b.minz; maxz = b.maxz;
}

float Box::GetWidth() const
{
    return (maxx-minx);
}

float Box::GetHeight() const
{
    return (maxy-miny);
}

float Box::GetDepth() const
{
    return (maxz-minz);
}

Vector3 Box::GetCenter() const
{
    return
       Vector3(
          (minx + maxx)/2,
          (miny + maxy)/2,
          (minz + maxz)/2
       );
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
    Box(std::min(b1.minx, b2.minx), std::max(b1.maxx, b2.maxx),
        std::min(b1.miny, b2.miny), std::max(b1.maxy, b2.maxy),
        std::min(b1.minz, b2.minz), std::max(b1.maxz, b2.maxz));
}

void Box::FillFromPositions(const std::vector<Vector3> &positions)
{
    minx = maxx = miny = maxy = minz = maxz = 0.0f;
    for(const Vector3 &v : positions)
    {
        minx = std::min(minx, v.x);
        maxx = std::max(maxx, v.x);

        miny = std::min(miny, v.y);
        maxy = std::max(maxy, v.y);

        minz = std::min(minz, v.z);
        maxz = std::max(maxz, v.z);
    }
}



Box operator*(const Matrix4 &m, const Box &b)
{
    glm::vec4 bMinCoords = glm::vec4(b.minx, b.miny, b.minz, 1.0f);
    glm::vec4 bMaxCoords = glm::vec4(b.maxx, b.maxy, b.maxz, 1.0f);
    glm::vec4 v1 = m * bMinCoords;
    glm::vec4 v2 = m * bMaxCoords;

    Box br(std::min(v1.x, v2.x), std::max(v1.x, v2.x),
           std::min(v1.y, v2.y), std::max(v1.y, v2.y),
           std::min(v1.z, v2.z), std::max(v1.z, v2.z));
    return br;
}


