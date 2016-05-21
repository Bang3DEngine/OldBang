#ifndef BOX_H
#define BOX_H

#include "Bang.h"

#include <cmath>
#include <vector>

#include "Vector3.h"
#include "Matrix4.h"

#include "IToString.h"

class Box : public IToString
{
private:
    Vector3 minv, maxv;

public:

    Box();
    Box(float minx, float maxx,
        float miny, float maxy,
        float minz, float maxz);
    Box(const Vector3 &min, const Vector3 &max);
    Box(const Box& b);

    Vector3 GetMin() const;
    Vector3 GetMax() const;
    Vector3 GetDiagonal() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetDepth() const;
    Vector3 GetCenter() const;
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
    void FillFromPositions(const std::vector<Vector3> &positions);

    const std::string ToString() const override;
};

Box operator*(const Matrix4 &m, const Box &b);

#endif // BOX_H