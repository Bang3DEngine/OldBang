#ifndef BOX_H
#define BOX_H

#include <cmath>

#include "List.h"
#include "Array.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "IToString.h"
#include "Quaternion.h"


class Rect;
class Sphere;
class Camera;
class Quaternion;
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

    Vector3 GetClosestPointInAABB(const Vector3 &point) const;
    bool CheckCollision(const Sphere &sphere) const;
    bool CheckCollision(const AABox &aabox) const;
    bool Contains(const Vector3 &point) const;

    /**
     * @brief For every coord, it gets the max and min values of b1 and b2.
     * For example
     * Union( b1(-1,-99,-1, 1,1,99),
     * b2(-99,-99,-2, 2,0,1) ) = b(-99,-99,-2, 2,1,99)
     * @param b1
     * @param b2
     * @return
     */
    static AABox Union(const AABox &b1, const AABox &b2);

    /**
     * @brief This Box fills its values with the
     * Bounding Box of the positions vector.
     * @param positions
     */
    void FillFromPositions(const Array<Vector3> &positions);

    static AABox FromSphere(const Sphere &sphere);

    Array<Vector3> GetPoints() const;

    /**
     * @brief Returns the minimum 2D rect in screen NDC space that contains
     * this box rendered from the passed camera.
     * @param cam
     * @return
     */
    Rect GetAABoundingScreenRect(Camera *cam) const;

    String ToString() const override;
};

AABox operator*(const Matrix4 &m, const AABox &b);
bool operator==(const AABox &b1, const AABox &b2);

#endif // BOX_H
