#ifndef BOX_H
#define BOX_H

#include <cmath>

#include "Bang.h"

#include "Array.h"
#include "List.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"

#include "IToString.h"

class Rect;
class Sphere;
class Camera;
class Quaternion;
class Box : public IToString
{
private:
    Vector3 m_minv, m_maxv;

public:
    static Box Empty;

    Box();
    Box(float minx, float maxx,
        float miny, float maxy,
        float minz, float maxz);
    Box(const Vector3 &min, const Vector3 &max);
    Box(const Box& b);

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

    /**
     * @brief For every coord, it gets the max and min values of b1 and b2.
     * For example
     * Union( b1(-1,-99,-1, 1,1,99),
     * b2(-99,-99,-2, 2,0,1) ) = b(-99,-99,-2, 2,1,99)
     * @param b1
     * @param b2
     * @return
     */
    static Box Union(const Box &b1, const Box &b2);

    /**
     * @brief This Box fills its values with the
     * Bounding Box of the positions vector.
     * @param positions
     */
    void FillFromPositions(const Array<Vector3> &positions);

    static Box FromSphere(const Sphere &sphere);

    List<Vector3> GetPoints() const;

    /**
     * @brief Returns the minimum 2D rect in screen NDC space that contains
     * this box rendered from the passed camera.
     * @param cam
     * @return
     */
    Rect GetBoundingScreenRect(Camera *cam,
                               const Vector3 &translation = Vector3::Zero,
                               const Quaternion& rotation = Quaternion::Identity,
                               const Vector3 &scale = Vector3::One);

    const String ToString() const override;
};

Box operator*(const Matrix4 &m, const Box &b);
bool operator==(const Box &b1, const Box &b2);

#endif // BOX_H
