#ifndef RECT_H
#define RECT_H

#include "Bang.h"

#include "IToString.h"

class Rect : public IToString
{
public:
    float m_minx = 0.0f;
    float m_maxx = 0.0f;
    float m_miny = 0.0f;
    float m_maxy = 0.0f;

    Rect();
    Rect(float minx, float maxx,
         float miny, float maxy);

    float GetWidth() const;
    float GetHeight() const;
    float GetArea() const;

    const String ToString() const override;
};

#endif // RECT_H
