#ifndef RECT_H
#define RECT_H

#include "Bang.h"

#include "IToString.h"

class Rect : public IToString
{
public:
    float minx = 0.0f;
    float maxx = 0.0f;
    float miny = 0.0f;
    float maxy = 0.0f;

    Rect();
    Rect(float minx, float maxx,
         float miny, float maxy);

    float GetWidth() const;
    float GetHeight() const;
    float GetArea() const;

    const std::string ToString() const override;
};

#endif // RECT_H
