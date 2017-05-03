#ifndef RECT_H
#define RECT_H

#include "Bang/List.h"
#include "Bang/IToString.h"

class AABox;
class Camera;
class Vector2;
class Rect : public IToString
{
public:
    static Rect ScreenRect;
    static Rect Empty;

    float m_minx = 0.0f;
    float m_maxx = 0.0f;
    float m_miny = 0.0f;
    float m_maxy = 0.0f;

    Rect();
    explicit Rect(float minx, float maxx,
                  float miny, float maxy);
    explicit Rect(const Vector2 &p1, const Vector2 &p2);

    void InvertY();

    Vector2 GetTopLeft() const;
    Vector2 GetTopRight() const;
    Vector2 GetBotLeft() const;
    Vector2 GetBotRight() const;

    Vector2 GetMin() const;
    Vector2 GetMax() const;

    float GetWidth() const;
    float GetHeight() const;
    float GetArea() const;
    Vector2 GetSize() const;
    Vector2 GetCenter() const;

    bool Contains(const Vector2 &p) const;

    /**
     * @brief Makes a union of the 2 rects into a single one.
     * It returns the minimum bounding Rect that contains r1 and r2.
     * @param r1
     * @param r2
     * @return The unified Rect
     */
    static Rect Union(const Rect &r1, const Rect &r2);

    /**
     * @brief Makes an intersection of the 2 rects.
     * It returns the intersecting rect between r1 and r2.
     * @param r1
     * @param r2
     * @return The unified Rect
     */
    static Rect Intersection(const Rect &r1, const Rect &r2);

    static Rect GetBoundingRectFromPositions(const List<Vector2> &positions);

    String ToString() const override;
};

Rect operator*(const Matrix4 &m, const Rect &r);
void operator*=(Rect &r, float a);
void operator/=(Rect &r, float a);
Rect operator/(float a, const Rect &r);
Rect operator/(const Rect &r, float a);
Rect operator+(const Vector2 &v, const Rect &r);
Rect operator+(const Rect &r, const Vector2 &v);
Rect operator+(float a, const Rect &r);
Rect operator+(const Rect &r, float a);
Rect operator/(const Vector2 &v, const Rect &r);
Rect operator/(const Rect &r, const Vector2 &v);
Rect operator*(float a, const Rect &r);
Rect operator*(const Rect &r, float a);
Rect operator*(const Vector2 &v, const Rect &r);
Rect operator*(const Rect &r, const Vector2 &v);
void operator*=(Rect &r, const Vector2 &v);
void operator/=(Rect &r, const Vector2 &v);
bool operator==(const Rect &r1, const Rect &r2);
bool operator!=(const Rect &r1, const Rect &r2);

#endif // RECT_H
