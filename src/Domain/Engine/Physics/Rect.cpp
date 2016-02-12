#include "Rect.h"

Rect::Rect() : left(0.0f),
               right(0.0f),
               bottom(0.0f),
               top(0.0f)
{
}

Rect::Rect(float left, float right, float bottom, float top) : left(left),
                                                               right(right),
                                                               bottom(bottom),
                                                               top(top)
{
}

float Rect::GetWidth() const
{
    return (right-left);
}

float Rect::GetHeight() const
{
    return (top-bottom);
}

float Rect::GetArea() const
{
    return (right-left) * (top-bottom);
}

const std::string Rect::ToString() const
{
    std::ostringstream oss;
    oss << "Rect: [" << std::endl <<
           "  left:" << left << std::endl <<
           "  right:" << right << std::endl <<
           "  bottom:" << bottom << std::endl <<
           "  top:" << top << std::endl <<
           "]" << std::endl;

    return oss.str();
}
