#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector4.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Vector3;
class Quaternion;
class Matrix4
{
public:
    mutable Vector4 c0, c1, c2, c3;

    Matrix4();
    Matrix4(float a);
    Matrix4(const glm::mat4 &m);
    Matrix4(const Vector4 &col0,
            const Vector4 &col1,
            const Vector4 &col2,
            const Vector4 &col3);

    Matrix4 Inversed() const;
    Matrix4 Transposed() const;

    float *GetFirstAddress() const;

    void SetScale(const Vector3 &scale);

    glm::mat4 ToGlmMat4() const;

    static Matrix4 Perspective(float fovY, float aspect,
                               float zNear, float zFar);

    static Matrix4 Ortho(float left, float right,
                         float bottom, float top,
                         float zNear, float zFar);

    static Matrix4 TranslateMatrix(const Vector3 &v);
    static Matrix4 RotateMatrix(const Quaternion &q);
    static Matrix4 ScaleMatrix(const Vector3 &v);

    Vector4& operator[](int i) const;

    static Matrix4 identity;
};

Matrix4 operator*(const Matrix4 &m1, const Matrix4& m2);

#endif // MATRIX4_H
