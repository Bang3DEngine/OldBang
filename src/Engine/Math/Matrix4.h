#ifndef MATRIX4_H
#define MATRIX4_H

#include "Bang/Vector3.h"
#include "Bang/Vector4.h"

#include "Bang/glm/glm.hpp"
#include "Bang/glm/gtx/transform.hpp"

class String;
class Quaternion;
class Matrix4
{
public:
    mutable Vector4 c0, c1, c2, c3; // Matrix columns from left to right

    Matrix4();
    Matrix4(float a);
    Matrix4(const glm::mat4 &m);
    Matrix4(const Vector4 &col0,
            const Vector4 &col1,
            const Vector4 &col2,
            const Vector4 &col3);
    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33);

    Matrix4 Inversed() const;
    Matrix4 Transposed() const;

    float *GetFirstAddress() const;

    void SetTranslate(const Vector3 &translate);
    void SetScale(const Vector3 &scale);

    glm::mat4 ToGlmMat4() const;
    String ToString() const;

    static Matrix4 Perspective(float fovY, float aspect,
                               float zNear, float zFar);

    static Matrix4 Ortho(float left, float right,
                         float bottom, float top,
                         float zNear, float zFar);

    static Matrix4 TranslateMatrix(const Vector3 &v);
    static Matrix4 RotateMatrix(const Quaternion &q);
    static Matrix4 ScaleMatrix(const Vector3 &v);

    Vector4& operator[](int i) const;

    static Matrix4 Identity;
};

Matrix4 operator*(const Matrix4 &m1, const Matrix4& m2);

template<class T>
Vector4G<T> operator*(const Matrix4 &m, const Vector4G<T> &v)
{
    return Vector4((m[0].x * v.x) + (m[1].x * v.y) + (m[2].x * v.z) + (m[3].x * v.w),
                   (m[0].y * v.x) + (m[1].y * v.y) + (m[2].y * v.z) + (m[3].y * v.w),
                   (m[0].z * v.x) + (m[1].z * v.y) + (m[2].z * v.z) + (m[3].z * v.w),
                   (m[0].w * v.x) + (m[1].w * v.y) + (m[2].w * v.z) + (m[3].w * v.w));
}

#endif // MATRIX4_H
