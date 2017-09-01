#ifndef BOX_H
#define BOX_H

#include <cmath>

#include "Bang/Rect.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"
#include "Bang/IToString.h"

FORWARD   class Camera;

class AABox : public IToString
{
private:
    Vector3 m_minv, m_maxv;

public:
    static AABox Empty;

    AABox();
    AABox(float minx, float maxx,
          float miny, float maxy,
          float minz, float maxz);
    AABox(const Vector3 &min, const Vector3 &max);
    AABox(const AABox& b);

    void SetMin(const Vector3& bMin);
    void SetMax(const Vector3& bMax);

    const Vector3& GetMin() const;
    const Vector3& GetMax() const;
    Vector3 GetDiagonal() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;
    Vector3 GetCenter() const;
    Vector3 GetDimensions() const;
    float GetArea() const;
    float GetVolume() const;
    Rect GetAABoundingScreenRect(Camera *cam) const;
    Vector3 GetClosestPointInAABB(const Vector3 &point) const;
    Array<Vector3> GetPoints() const;

    bool CheckCollision(const Sphere &sphere,
                        Vector3 *point  = nullptr,
                        Vector3 *normal = nullptr) const;
    bool CheckCollision(const AABox &aabox) const;
    bool Contains(const Vector3 &point) const;

    static AABox Union(const AABox &b1, const AABox &b2);
    void FillFromPositions(const Array<Vector3> &positions);

    static AABox FromSphere(const Sphere &sphere);

    // IToString
    String ToString() const override;
};

AABox operator*(const Matrix4 &m, const AABox &b);
bool operator==(const AABox &b1, const AABox &b2);

#endif // BOX_H
