#include "Box.h"

Box::Box()
{
}

const std::string Box::ToString() const
{
    std::ostringstream oss;
    oss << "Box: [" << std::endl <<
           "  minx:" << minx << std::endl <<
           "  maxx:" << maxx << std::endl <<
           "  miny:" << miny << std::endl <<
           "  maxy:" << maxy << std::endl <<
           "  minz:" << minz << std::endl <<
           "  maxz:" << maxz << std::endl <<
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

glm::vec3 Box::GetCenter() const
{
    return
       glm::vec3(
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

void Box::FillFromBoundingBox(const std::vector<glm::vec3> &positions)
{
    minx = maxx = miny = maxy = minz = maxz = 0.0f;
    for(const glm::vec3 &v : positions)
    {
        minx = std::min(minx, v.x);
        maxx = std::max(maxx, v.x);

        miny = std::min(miny, v.y);
        maxy = std::max(maxy, v.y);

        minz = std::min(minz, v.z);
        maxz = std::max(maxz, v.z);
    }
}

