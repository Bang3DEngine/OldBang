#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(float minx, float maxx,
           float miny, float maxy) :
    minx(minx), maxx(maxx),
    miny(miny), maxy(maxy)
{
}

float Rect::GetWidth() const
{
    return (maxx-minx);
}

float Rect::GetHeight() const
{
    return (maxy-miny);
}

float Rect::GetArea() const
{
    return (maxx-minx) * (maxy-miny);
}

const std::string Rect::ToString() const
{
    std::ostringstream oss;
    oss << "Rect: [" << std::endl <<
           "  minx:" << minx << std::endl <<
           "  maxx:" << maxx << std::endl <<
           "  miny:" << miny << std::endl <<
           "  maxy:" << maxy << std::endl <<
           "]" << std::endl;

    return oss.str();
}
