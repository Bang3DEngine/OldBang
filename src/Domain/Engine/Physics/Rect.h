#ifndef RECT_H
#define RECT_H

#include "IToString.h"

class Rect : public IToString
{
public:
    float left, right, bottom, top;

    Rect();
    Rect(float left, float right, float bottom, float top);

    float GetWidth() const;
    float GetHeight() const;
    float GetArea() const;

    const std::string ToString() const override;
};

#endif // RECT_H
