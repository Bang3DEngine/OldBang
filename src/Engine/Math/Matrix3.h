#ifndef MATRIX3_H
#define MATRIX3_H

#include "Bang/Vector3.h"

// Part of it copied from glm

class String;
class Matrix3
{
public:
    mutable Vector3 c0, c1, c2; // Matrix columns from left to right

    Matrix3();
    Matrix3(float a);
    Matrix3(const Vector3 &col0,
            const Vector3 &col1,
            const Vector3 &col2);
    Matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22);

    Matrix3 Inversed() const;
    Matrix3 Transposed() const;

    float *Data() const;

    String ToString() const;

    Vector3& operator[](int i) const;

    static Matrix3 Identity;
};

#endif // MATRIX3_H
