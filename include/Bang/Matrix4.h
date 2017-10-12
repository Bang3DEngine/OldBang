#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>

#include "Bang/Debug.h"
#include "Bang/Vector4.h"

NAMESPACE_BANG_BEGIN

template<class T> // Part of it copied from glm
class Matrix4G
{
public:
    Vector4G<T> c0, c1, c2, c3; // Matrix columns from left to right

    Matrix4G() : Matrix4G(1)
    {
    }

    template<class OtherT>
    Matrix4G(const Matrix4G<OtherT>& m)
    {
        c0 = Vector4G<T>(m.c0);
        c1 = Vector4G<T>(m.c1);
        c2 = Vector4G<T>(m.c2);
        c3 = Vector4G<T>(m.c3);
    }

    template<class OtherT>
    Matrix4G(const OtherT& a)
    {
        c0 = Vector4G<T>(SCAST<T>(a), SCAST<T>(0), SCAST<T>(0), SCAST<T>(0));
        c1 = Vector4G<T>(SCAST<T>(0), SCAST<T>(a), SCAST<T>(0), SCAST<T>(0));
        c2 = Vector4G<T>(SCAST<T>(0), SCAST<T>(0), SCAST<T>(a), SCAST<T>(0));
        c3 = Vector4G<T>(SCAST<T>(0), SCAST<T>(0), SCAST<T>(0), SCAST<T>(a));
    }
    Matrix4G(const Vector4G<T> &col0,
             const Vector4G<T> &col1,
             const Vector4G<T> &col2,
             const Vector4G<T> &col3)
    {
        c0 = col0;
        c1 = col1;
        c2 = col2;
        c3 = col3;
    }
    Matrix4G(const T& m00, const T& m01, const T& m02, const T& m03,
             const T& m10, const T& m11, const T& m12, const T& m13,
             const T& m20, const T& m21, const T& m22, const T& m23,
             const T& m30, const T& m31, const T& m32, const T& m33)
    {
        c0 = Vector4G<T>(m00, m10, m20, m30);
        c1 = Vector4G<T>(m01, m11, m21, m31);
        c2 = Vector4G<T>(m02, m12, m22, m32);
        c3 = Vector4G<T>(m03, m13, m23, m33);
    }

    Matrix4G<T> Inversed() const
    {
        Matrix4G<T> inv;
        const Matrix4G<T> &m = *this;

        inv[0].x = m[1].y * m[2].z * m[3].w -
                   m[1].y * m[2].w * m[3].z -
                   m[2].y * m[1].z * m[3].w +
                   m[2].y * m[1].w * m[3].z +
                   m[3].y * m[1].z * m[2].w -
                   m[3].y * m[1].w * m[2].z;

        inv[1].x = -m[1].x * m[2].z * m[3].w +
                    m[1].x * m[2].w * m[3].z +
                    m[2].x * m[1].z * m[3].w -
                    m[2].x * m[1].w * m[3].z -
                    m[3].x * m[1].z * m[2].w +
                    m[3].x * m[1].w * m[2].z;

        inv[2].x = m[1].x * m[2].y * m[3].w -
                   m[1].x * m[2].w * m[3].y -
                   m[2].x * m[1].y * m[3].w +
                   m[2].x * m[1].w * m[3].y +
                   m[3].x * m[1].y * m[2].w -
                   m[3].x * m[1].w * m[2].y;

        inv[3].x = -m[1].x * m[2].y * m[3].z +
                    m[1].x * m[2].z * m[3].y +
                    m[2].x * m[1].y * m[3].z -
                    m[2].x * m[1].z * m[3].y -
                    m[3].x * m[1].y * m[2].z +
                    m[3].x * m[1].z * m[2].y;

        inv[0].y = -m[0].y * m[2].z * m[3].w +
                    m[0].y * m[2].w * m[3].z +
                    m[2].y * m[0].z * m[3].w -
                    m[2].y * m[0].w * m[3].z -
                    m[3].y * m[0].z * m[2].w +
                    m[3].y * m[0].w * m[2].z;

        inv[1].y = m[0].x * m[2].z * m[3].w -
                   m[0].x * m[2].w * m[3].z -
                   m[2].x * m[0].z * m[3].w +
                   m[2].x * m[0].w * m[3].z +
                   m[3].x * m[0].z * m[2].w -
                   m[3].x * m[0].w * m[2].z;

        inv[2].y = -m[0].x * m[2].y * m[3].w +
                    m[0].x * m[2].w * m[3].y +
                    m[2].x * m[0].y * m[3].w -
                    m[2].x * m[0].w * m[3].y -
                    m[3].x * m[0].y * m[2].w +
                    m[3].x * m[0].w * m[2].y;

        inv[3].y = m[0].x * m[2].y * m[3].z -
                   m[0].x * m[2].z * m[3].y -
                   m[2].x * m[0].y * m[3].z +
                   m[2].x * m[0].z * m[3].y +
                   m[3].x * m[0].y * m[2].z -
                   m[3].x * m[0].z * m[2].y;

        inv[0].z = m[0].y * m[1].z * m[3].w -
                   m[0].y * m[1].w * m[3].z -
                   m[1].y * m[0].z * m[3].w +
                   m[1].y * m[0].w * m[3].z +
                   m[3].y * m[0].z * m[1].w -
                   m[3].y * m[0].w * m[1].z;

        inv[1].z = -m[0].x * m[1].z * m[3].w +
                    m[0].x * m[1].w * m[3].z +
                    m[1].x * m[0].z * m[3].w -
                    m[1].x * m[0].w * m[3].z -
                    m[3].x * m[0].z * m[1].w +
                    m[3].x * m[0].w * m[1].z;

        inv[2].z = m[0].x * m[1].y * m[3].w -
                   m[0].x * m[1].w * m[3].y -
                   m[1].x * m[0].y * m[3].w +
                   m[1].x * m[0].w * m[3].y +
                   m[3].x * m[0].y * m[1].w -
                   m[3].x * m[0].w * m[1].y;

        inv[3].z = -m[0].x * m[1].y * m[3].z +
                    m[0].x * m[1].z * m[3].y +
                    m[1].x * m[0].y * m[3].z -
                    m[1].x * m[0].z * m[3].y -
                    m[3].x * m[0].y * m[1].z +
                    m[3].x * m[0].z * m[1].y;

        inv[0].w = -m[0].y * m[1].z * m[2].w +
                    m[0].y * m[1].w * m[2].z +
                    m[1].y * m[0].z * m[2].w -
                    m[1].y * m[0].w * m[2].z -
                    m[2].y * m[0].z * m[1].w +
                    m[2].y * m[0].w * m[1].z;

        inv[1].w = m[0].x * m[1].z * m[2].w -
                   m[0].x * m[1].w * m[2].z -
                   m[1].x * m[0].z * m[2].w +
                   m[1].x * m[0].w * m[2].z +
                   m[2].x * m[0].z * m[1].w -
                   m[2].x * m[0].w * m[1].z;

        inv[2].w = -m[0].x * m[1].y * m[2].w +
                    m[0].x * m[1].w * m[2].y +
                    m[1].x * m[0].y * m[2].w -
                    m[1].x * m[0].w * m[2].y -
                    m[2].x * m[0].y * m[1].w +
                    m[2].x * m[0].w * m[1].y;

        inv[3].w = m[0].x * m[1].y * m[2].z -
                   m[0].x * m[1].z * m[2].y -
                   m[1].x * m[0].y * m[2].z +
                   m[1].x * m[0].z * m[2].y +
                   m[2].x * m[0].y * m[1].z -
                   m[2].x * m[0].z * m[1].y;

        float det = m[0].x * inv[0].x + m[0].y * inv[1].x + m[0].z * inv[2].x +
                    m[0].w * inv[3].x;
        if (det == 0) return *this;

        inv.c0 *= SCAST<T>(1) / det;
        inv.c1 *= SCAST<T>(1) / det;
        inv.c2 *= SCAST<T>(1) / det;
        inv.c3 *= SCAST<T>(1) / det;
        return inv;
    }

    Matrix4G<T> Transposed() const
    {
        Matrix4G<T> trans;
        const Matrix4G<T> &m = *this;

        trans[0].x = m[0].x;
        trans[0].y = m[1].x;
        trans[0].z = m[2].x;
        trans[0].w = m[3].x;

        trans[1].x = m[0].y;
        trans[1].y = m[1].y;
        trans[1].z = m[2].y;
        trans[1].w = m[3].y;

        trans[2].x = m[0].z;
        trans[2].y = m[1].z;
        trans[2].z = m[2].z;
        trans[2].w = m[3].z;

        trans[3].x = m[0].w;
        trans[3].y = m[1].w;
        trans[3].z = m[2].w;
        trans[3].w = m[3].w;

        return trans;
    }

    T *Data() { return SCAST<T*>(&(c0.x)); }
    const T *Data() const { return SCAST<const T*>(&(c0.x)); }

    void SetTranslate(const Vector3G<T> &translate)
    {
        c3.x = translate.x;
        c3.y = translate.y;
        c3.z = translate.z;
    }
    void SetScale(const Vector3G<T> &scale)
    {
        c0.x = scale.x;
        c1.y = scale.y;
        c2.z = scale.z;
    }

    static Matrix4G<T> LookAt(const Vector3G<T> &eyePosition,
                              const Vector3G<T> &focusPoint,
                              const Vector3G<T> &up)
    {
        const Vector3G<T> f( (focusPoint - eyePosition).Normalized() );
        const Vector3G<T> s( (Vector3G<T>::Cross(f, up)).Normalized() );
        const Vector3G<T> u(  Vector3G<T>::Cross(s, f) );

        Matrix4G<T> res(1);
        res[0][0] =  s.x;
        res[1][0] =  s.y;
        res[2][0] =  s.z;
        res[0][1] =  u.x;
        res[1][1] =  u.y;
        res[2][1] =  u.z;
        res[0][2] = -f.x;
        res[1][2] = -f.y;
        res[2][2] = -f.z;
        res[3][0] = -Vector3G<T>::Dot(s, eyePosition);
        res[3][1] = -Vector3G<T>::Dot(u, eyePosition);
        res[3][2] =  Vector3G<T>::Dot(f, eyePosition);
        return res;
    }

    template <class Real>
    static Matrix4G<T> Perspective(Real fovYRads,
                                   Real aspect,
                                   Real zNear,
                                   Real zFar)
    {
        float const tanHalfFovy = Math::Tan(fovYRads / SCAST<T>(2));

        Matrix4G<T> res(0);
        res[0][0] = SCAST<T>(1) / (aspect * tanHalfFovy);
        res[1][1] = SCAST<T>(1) / (tanHalfFovy);
        res[2][2] = -(zFar + zNear) / (zFar - zNear);
        res[2][3] = -SCAST<T>(1);
        res[3][2] = -(SCAST<T>(2) * zFar * zNear) / (zFar - zNear);

        return res;
    }

    template <class Real>
    static Matrix4G<T> Ortho(Real left,   Real right,
                             Real bottom, Real top,
                             Real zNear,  Real zFar)
    {
        Matrix4G<T> res(1);
        res[0][0] = SCAST<T>(2) / (right - left);
        res[1][1] = SCAST<T>(2) / (top - bottom);
        res[2][2] = -SCAST<T>(2) / (zFar - zNear);
        res[3][0] = -(right + left) / (right - left);
        res[3][1] = -(top + bottom) / (top - bottom);
        res[3][2] = -(zFar + zNear) / (zFar - zNear);
        return res;
    }

    static Matrix4G<T> TranslateMatrix(const Vector3G<T> &v)
    {
        return Matrix4G<T>(1, 0, 0, v.x,
                           0, 1, 0, v.y,
                           0, 0, 1, v.z,
                           0, 0, 0, 1);
    }

    static QuaternionG<T> ToQuaternion(const Matrix4G<T> &m)
    {
        T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
        T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
        T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
        T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

        int biggestIndex = 0;
        T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
        if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourXSquaredMinus1;
            biggestIndex = 1;
        }
        if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourYSquaredMinus1;
            biggestIndex = 2;
        }
        if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
        {
            fourBiggestSquaredMinus1 = fourZSquaredMinus1;
            biggestIndex = 3;
        }

        float biggestVal = Math::Sqrt(fourBiggestSquaredMinus1 + SCAST<T>(1)) *
                           SCAST<T>(0.5);
        float mult = SCAST<T>(0.25)/biggestVal;

        QuaternionG<T> res;
        switch (biggestIndex)
        {
            case 0:
                res.x = (m[1][2] - m[2][1]) * mult;
                res.y = (m[2][0] - m[0][2]) * mult;
                res.z = (m[0][1] - m[1][0]) * mult;
                res.w = biggestVal;
                break;
            case 1:
                res.x = biggestVal;
                res.y = (m[0][1] + m[1][0]) * mult;
                res.z = (m[2][0] + m[0][2]) * mult;
                res.w = (m[1][2] - m[2][1]) * mult;
                break;
            case 2:
                res.x = (m[0][1] + m[1][0]) * mult;
                res.y = biggestVal;
                res.z = (m[1][2] + m[2][1]) * mult;
                res.w = (m[2][0] - m[0][2]) * mult;
                break;
            case 3:
                res.x = (m[2][0] + m[0][2]) * mult;
                res.y = (m[1][2] + m[2][1]) * mult;
                res.z = biggestVal;
                res.w = (m[0][1] - m[1][0]) * mult;
                break;

            default: break;
        }
        return res;
    }

    static Matrix4G<T> RotateMatrix(const QuaternionG<T> &q)
    {
        T qxx(q.x * q.x);
        T qyy(q.y * q.y);
        T qzz(q.z * q.z);
        T qxz(q.x * q.z);
        T qxy(q.x * q.y);
        T qyz(q.y * q.z);
        T qwx(q.w * q.x);
        T qwy(q.w * q.y);
        T qwz(q.w * q.z);

        Matrix4G<T> res(1);

        res[0][0] = 1 - 2 * (qyy +  qzz);
        res[0][1] = 2 * (qxy + qwz);
        res[0][2] = 2 * (qxz - qwy);

        res[1][0] = 2 * (qxy - qwz);
        res[1][1] = 1 - 2 * (qxx +  qzz);
        res[1][2] = 2 * (qyz + qwx);

        res[2][0] = 2 * (qxz + qwy);
        res[2][1] = 2 * (qyz - qwx);
        res[2][2] = 1 - 2 * (qxx +  qyy);

        return res;
    }
    static Matrix4G<T> ScaleMatrix(const Vector3G<T> &v)
    {
        return Matrix4G<T>(v.x, 0,   0,  0,
                           0, v.y,   0,  0,
                           0,   0, v.z,  0,
                           0,   0,   0,  1);
    }

    Vector4G<T>& operator[](::std::size_t i)
    {
        switch (i)
        {
            case 0: return c0;
            case 1: return c1;
            case 2: return c2;
            case 3: return c3;
        }
        Debug_Warn("Matrix4G<T> index " << i << " too big");
        return c3;
    }
    const Vector4G<T>& operator[](::std::size_t i) const
    {
        return const_cast< Matrix4G<T>* >(this)->operator[](i);
    }

    static const Matrix4G Identity;
};

template<class T>
const Matrix4G<T> Matrix4G<T>::Identity = Matrix4G<T>(1);

template<class T>
Matrix4G<T> operator*(const Matrix4G<T> &m1, const Matrix4G<T>& m2)
{
    Matrix4G<T> m;
    m.c0 = Vector4G<T>((m1.c0.x * m2.c0.x) + (m1.c1.x * m2.c0.y) + (m1.c2.x * m2.c0.z) + (m1.c3.x * m2.c0.w),
                       (m1.c0.y * m2.c0.x) + (m1.c1.y * m2.c0.y) + (m1.c2.y * m2.c0.z) + (m1.c3.y * m2.c0.w),
                       (m1.c0.z * m2.c0.x) + (m1.c1.z * m2.c0.y) + (m1.c2.z * m2.c0.z) + (m1.c3.z * m2.c0.w),
                       (m1.c0.w * m2.c0.x) + (m1.c1.w * m2.c0.y) + (m1.c2.w * m2.c0.z) + (m1.c3.w * m2.c0.w));

    m.c1 = Vector4G<T>((m1.c0.x * m2.c1.x) + (m1.c1.x * m2.c1.y) + (m1.c2.x * m2.c1.z) + (m1.c3.x * m2.c1.w),
                       (m1.c0.y * m2.c1.x) + (m1.c1.y * m2.c1.y) + (m1.c2.y * m2.c1.z) + (m1.c3.y * m2.c1.w),
                       (m1.c0.z * m2.c1.x) + (m1.c1.z * m2.c1.y) + (m1.c2.z * m2.c1.z) + (m1.c3.z * m2.c1.w),
                       (m1.c0.w * m2.c1.x) + (m1.c1.w * m2.c1.y) + (m1.c2.w * m2.c1.z) + (m1.c3.w * m2.c1.w));

    m.c2 = Vector4G<T>((m1.c0.x * m2.c2.x) + (m1.c1.x * m2.c2.y) + (m1.c2.x * m2.c2.z) + (m1.c3.x * m2.c2.w),
                       (m1.c0.y * m2.c2.x) + (m1.c1.y * m2.c2.y) + (m1.c2.y * m2.c2.z) + (m1.c3.y * m2.c2.w),
                       (m1.c0.z * m2.c2.x) + (m1.c1.z * m2.c2.y) + (m1.c2.z * m2.c2.z) + (m1.c3.z * m2.c2.w),
                       (m1.c0.w * m2.c2.x) + (m1.c1.w * m2.c2.y) + (m1.c2.w * m2.c2.z) + (m1.c3.w * m2.c2.w));

    m.c3 = Vector4G<T>((m1.c0.x * m2.c3.x) + (m1.c1.x * m2.c3.y) + (m1.c2.x * m2.c3.z) + (m1.c3.x * m2.c3.w),
                       (m1.c0.y * m2.c3.x) + (m1.c1.y * m2.c3.y) + (m1.c2.y * m2.c3.z) + (m1.c3.y * m2.c3.w),
                       (m1.c0.z * m2.c3.x) + (m1.c1.z * m2.c3.y) + (m1.c2.z * m2.c3.z) + (m1.c3.z * m2.c3.w),
                       (m1.c0.w * m2.c3.x) + (m1.c1.w * m2.c3.y) + (m1.c2.w * m2.c3.z) + (m1.c3.w * m2.c3.w));
    return m;
}


template<class T>
Vector4G<T> operator*(const Matrix4G<T> &m, const Vector4G<T> &v)
{
    return Vector4G<T>((m[0].x * v.x) + (m[1].x * v.y) + (m[2].x * v.z) + (m[3].x * v.w),
                       (m[0].y * v.x) + (m[1].y * v.y) + (m[2].y * v.z) + (m[3].y * v.w),
                       (m[0].z * v.x) + (m[1].z * v.y) + (m[2].z * v.z) + (m[3].z * v.w),
                       (m[0].w * v.x) + (m[1].w * v.y) + (m[2].w * v.z) + (m[3].w * v.w));
}

NAMESPACE_BANG_END

#endif // MATRIX4_H
