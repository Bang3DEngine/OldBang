#ifndef MATRIX4_H
#define MATRIX4_H

#include "Bang/Matrix3.h"
#include "Bang/Vector4.h"

NAMESPACE_BANG_BEGIN

template<class T> // Part of it copied from glm
class Matrix4G
{
public:
    static const Matrix4G Identity;

    Vector4G<T> c0, c1, c2, c3; // Matrix columns from left to right

    Matrix4G();

    template<class OtherT>
    Matrix4G(const Matrix4G<OtherT>& m);

    Matrix4G(const Vector4G<T> &col0,
             const Vector4G<T> &col1,
             const Vector4G<T> &col2,
             const Vector4G<T> &col3);

    Matrix4G(const T& m00, const T& m01, const T& m02, const T& m03,
             const T& m10, const T& m11, const T& m12, const T& m13,
             const T& m20, const T& m21, const T& m22, const T& m23,
             const T& m30, const T& m31, const T& m32, const T& m33);

    template<class OtherT>
    explicit Matrix4G(const Matrix3G<OtherT>& m);

    template<class OtherT>
    explicit Matrix4G(const OtherT& a);

    Vector3 TransformPoint(const Vector3 &point) const;
    Vector3 TransformVector(const Vector3 &vector) const;

    Matrix4G<T> Inversed() const;
    Matrix4G<T> Transposed() const;

    T *Data();
    const T *Data() const;

    void SetTranslate(const Vector3G<T> &translate);
    void SetScale(const Vector3G<T> &scale);

    static Matrix4G<T> LookAt(const Vector3G<T> &eyePosition,
                              const Vector3G<T> &focusPoint,
                              const Vector3G<T> &up);

    template <class Real>
    static Matrix4G<T> Perspective(Real fovYRads,
                                   Real aspect,
                                   Real zNear,
                                   Real zFar);
    template <class Real>
    static Matrix4G<T> Ortho(Real left,   Real right,
                             Real bottom, Real top,
                             Real zNear,  Real zFar);

    static Matrix4G<T> TranslateMatrix(const Vector3G<T> &v);
    static Matrix4G<T> RotateMatrix(const QuaternionG<T> &q);
    static Matrix4G<T> ScaleMatrix(const Vector3G<T> &v);
    static QuaternionG<T> ToQuaternion(const Matrix4G<T> &m);

    Vector4G<T>& operator[](std::size_t i);
    const Vector4G<T>& operator[](std::size_t i) const;
};

// Operators
template<class T>
bool operator==(const Matrix4G<T> &m1, const Matrix4G<T>& m2);

template<class T>
bool operator!=(const Matrix4G<T> &m1, const Matrix4G<T>& m2);

template<class T>
Matrix4G<T> operator*(const Matrix4G<T> &m1, const Matrix4G<T>& m2);

template<class T>
Vector4G<T> operator*(const Matrix4G<T> &m, const Vector4G<T> &v);

NAMESPACE_BANG_END

#include "Bang/Matrix4.tcc"

#endif // MATRIX4_H
