#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>

#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

template<class T> // Part of it copied from glm
class Matrix3G
{
public:
    Vector3G<T> c0, c1, c2; // Matrix columns from left to right

    Matrix3G() : Matrix3G<T>(1)
    {
    }

    template<class OtherT>
    Matrix3G(const OtherT& a)
    {
        c0 = Vector3(SCAST<T>(a), SCAST<T>(0), SCAST<T>(0));
        c1 = Vector3(SCAST<T>(0), SCAST<T>(a), SCAST<T>(0));
        c2 = Vector3(SCAST<T>(0), SCAST<T>(0), SCAST<T>(a));
    }

    Matrix3G(const Vector3G<T> &col0,
             const Vector3G<T> &col1,
             const Vector3G<T> &col2)
    {
        c0 = col0;
        c1 = col1;
        c2 = col2;
    }

    Matrix3G(const T& m00, const T& m01, const T& m02,
             const T& m10, const T& m11, const T& m12,
             const T& m20, const T& m21, const T& m22)
    {
        c0 = Vector3(m00, m10, m20);
        c1 = Vector3(m01, m11, m21);
        c2 = Vector3(m02, m12, m22);
    }

    Matrix3G<T> Inversed() const
    {
        const Matrix3G<T> &m = *this;
        const T invDet = SCAST<T>(1) / (
                + m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
                - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
                + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

        Matrix3G<T> res;
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

    Matrix3G<T> Transposed() const
    {
        Matrix3G<T> trans;
        const Matrix3G<T> &m = *this;

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

    const T *Data() const { return SCAST<const T*>(&(c0.x)); }
    T *Data() { return SCAST<T*>(&(c0.x)); }

    Vector3G<T>& operator[](std::size_t i)
    {
        switch (i)
        {
            case 0: return c0;
            case 1: return c1;
            case 2: return c2;
        }
        std::cerr << "Matrix3G<T> index " << i << " too big" << std::endl;
        return c2;
    }
    const Vector3G<T>& operator[](std::size_t i) const
    {
        return const_cast< Matrix3G<T>* >(this)->operator[](i);
    }

    const static Matrix3G<T> Identity;
};

template<class T>
const Matrix3G<T> Matrix3G<T>::Identity = Matrix3G<T>(1);

template<class T, class OtherT>
Matrix3G<T> operator*(const Matrix3G<T> &m1, const Matrix3G<OtherT> &m2)
{
    const T vA00 = m1[0][0];
    const T vA01 = m1[0][1];
    const T vA02 = m1[0][2];
    const T vA10 = m1[1][0];
    const T vA11 = m1[1][1];
    const T vA12 = m1[1][2];
    const T vA20 = m1[2][0];
    const T vA21 = m1[2][1];
    const T vA22 = m1[2][2];

    const T vB00 = SCAST<T>(m2[0][0]);
    const T vB01 = SCAST<T>(m2[0][1]);
    const T vB02 = SCAST<T>(m2[0][2]);
    const T vB10 = SCAST<T>(m2[1][0]);
    const T vB11 = SCAST<T>(m2[1][1]);
    const T vB12 = SCAST<T>(m2[1][2]);
    const T vB20 = SCAST<T>(m2[2][0]);
    const T vB21 = SCAST<T>(m2[2][1]);
    const T vB22 = SCAST<T>(m2[2][2]);

    Matrix3G<T> res;
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

NAMESPACE_BANG_END

#endif // MATRIX3_H
