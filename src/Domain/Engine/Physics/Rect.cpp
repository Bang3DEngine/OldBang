#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(float minx, float maxx,
           float miny, float maxy) :
    m_minx(minx), m_maxx(maxx),
    m_miny(miny), m_maxy(maxy)
{
}

float Rect::GetWidth() const
{
    return (m_maxx-m_minx);
}

float Rect::GetHeight() const
{
    return (m_maxy-m_miny);
}

float Rect::GetArea() const
{
    return (m_maxx-m_minx) * (m_maxy-m_miny);
}

const std::string Rect::ToString() const
{
    std::ostringstream oss;
    oss << "Rect: [" << std::endl <<
           "  minx:" << m_minx << std::endl <<
           "  maxx:" << m_maxx << std::endl <<
           "  miny:" << m_miny << std::endl <<
           "  maxy:" << m_maxy << std::endl <<
           "]" << std::endl;

    return oss.str();
}
