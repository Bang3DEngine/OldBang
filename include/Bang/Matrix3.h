#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>

#include "Bang/Vector3.h"

NAMESPACE_BANG_BEGIN

template<class T> // Part of it copied from glm
class Matrix3G
{
public:
    const static Matrix3G<T> Identity;

    Vector3G<T> c0, c1, c2; // Matrix columns from left to right

    Matrix3G();

    template<class OtherT>
    Matrix3G(const OtherT& a);

    Matrix3G(const Vector3G<T> &col0,
             const Vector3G<T> &col1,
             const Vector3G<T> &col2);

    Matrix3G(const T& m00, const T& m01, const T& m02,
             const T& m10, const T& m11, const T& m12,
             const T& m20, const T& m21, const T& m22);

    Matrix3G<T> Inversed() const;
    Matrix3G<T> Transposed() const;

    T *Data();
    const T *Data() const;

    Vector3G<T>& operator[](std::size_t i);
    const Vector3G<T>& operator[](std::size_t i) const;
};

// Operators
template<class T, class OtherT>
Matrix3G<T> operator*(const Matrix3G<T> &m1, const Matrix3G<OtherT> &m2);

template<class T>
bool operator==(const Matrix3G<T> &m1, const Matrix3G<T>& m2);

template<class T>
bool operator!=(const Matrix3G<T> &m1, const Matrix3G<T>& m2);

NAMESPACE_BANG_END

#include "Bang/Matrix3.tcc"

#endif // MATRIX3_H
