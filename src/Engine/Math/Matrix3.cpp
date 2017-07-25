#include "Bang/Matrix3.h"

#include <sstream>

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"

Matrix3 Matrix3::Identity = Matrix3();

Matrix3::Matrix3() : Matrix3(1.0f)
{
}

Matrix3::Matrix3(float a)
{
    c0 = Vector3(a, 0, 0);
    c1 = Vector3(0, a, 0);
    c2 = Vector3(0, 0, a);
}

Matrix3::Matrix3(const Vector3 &col0,
                 const Vector3 &col1,
                 const Vector3 &col2)
{
    c0 = col0;
    c1 = col1;
    c2 = col2;
}

Matrix3::Matrix3(float m00, float m01, float m02,
                 float m10, float m11, float m12,
                 float m20, float m21, float m22)
{
    c0 = Vector3(m00, m10, m20);
    c1 = Vector3(m01, m11, m21);
    c2 = Vector3(m02, m12, m22);
}

Matrix3 Matrix3::Inversed() const
{
    const Matrix3 &m = *this;
    const float invDet = SCAST<float>(1) / (
            + m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
            - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
            + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

    Matrix3 res;
    res[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
    res[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
    res[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
    res[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
    res[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
    res[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
    res[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
    res[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
    res[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;
    return res;
}

Matrix3 Matrix3::Transposed() const
{
    Matrix3 trans;
    const Matrix3 &m = *this;

    trans[0].x = m[0].x;
    trans[0].y = m[1].x;
    trans[0].z = m[2].x;

    trans[1].x = m[0].y;
    trans[1].y = m[1].y;
    trans[1].z = m[2].y;

    trans[2].x = m[0].z;
    trans[2].y = m[1].z;
    trans[2].z = m[2].z;

    return trans;
}

float *Matrix3::Data() const
{
    return SCAST<float*>(&(c0.x));
}

String Matrix3::ToString() const
{
    std::ostringstream oss;
    oss << "(" << c0[0] << ", " << c1[0] << ", " << c2[0] << "," << std::endl;
    oss << " " << c0[1] << ", " << c1[1] << ", " << c2[1] << "," << std::endl;
    oss << " " << c0[2] << ", " << c1[2] << ", " << c2[2] << "," << std::endl;
    oss << " " << c0[3] << ", " << c1[3] << ", " << c2[3] << ")" << std::endl;
    return String(oss.str());
}

Vector3& Matrix3::operator[](int i) const
{
    switch (i)
    {
        case 0: return c0;
        case 1: return c1;
        case 2: return c2;
    }
    Debug_Warn("Matrix3 index " << i << " too big");
    return c2;
}

Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2)
{
    const float vA00 = m1[0][0];
    const float vA01 = m1[0][1];
    const float vA02 = m1[0][2];
    const float vA10 = m1[1][0];
    const float vA11 = m1[1][1];
    const float vA12 = m1[1][2];
    const float vA20 = m1[2][0];
    const float vA21 = m1[2][1];
    const float vA22 = m1[2][2];

    const float vB00 = m2[0][0];
    const float vB01 = m2[0][1];
    const float vB02 = m2[0][2];
    const float vB10 = m2[1][0];
    const float vB11 = m2[1][1];
    const float vB12 = m2[1][2];
    const float vB20 = m2[2][0];
    const float vB21 = m2[2][1];
    const float vB22 = m2[2][2];

    Matrix3 res;
    res[0][0] = vA00 * vB00 + vA10 * vB01 + vA20 * vB02;
    res[0][1] = vA01 * vB00 + vA11 * vB01 + vA21 * vB02;
    res[0][2] = vA02 * vB00 + vA12 * vB01 + vA22 * vB02;
    res[1][0] = vA00 * vB10 + vA10 * vB11 + vA20 * vB12;
    res[1][1] = vA01 * vB10 + vA11 * vB11 + vA21 * vB12;
    res[1][2] = vA02 * vB10 + vA12 * vB11 + vA22 * vB12;
    res[2][0] = vA00 * vB20 + vA10 * vB21 + vA20 * vB22;
    res[2][1] = vA01 * vB20 + vA11 * vB21 + vA21 * vB22;
    res[2][2] = vA02 * vB20 + vA12 * vB21 + vA22 * vB22;
    return res;
}
