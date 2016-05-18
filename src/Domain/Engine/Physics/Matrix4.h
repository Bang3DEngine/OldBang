#ifndef MATRIX4_H
#define MATRIX4_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

class Vector3;
class Quaternion;
class Matrix4 : public glm::mat4
{
public:
    Matrix4();
    explicit Matrix4(const glm::mat4 &m);
    Matrix4(float a);

    Matrix4 Inversed() const;
    Matrix4 Transposed() const;

    float *GetFirstAddress() const;

    static Matrix4 Perspective(float fovY, float aspect,
                               float zNear, float zFar);

    static Matrix4 Ortho(float left, float right,
                         float bottom, float top,
                         float zNear, float zFar);

    static Matrix4 TranslateMatrix(const Vector3 &v);
    static Matrix4 RotateMatrix(const Quaternion &q);
    static Matrix4 ScaleMatrix(const Vector3 &v);
};

Matrix4 operator*(const Matrix4 &m1, const Matrix4& m2);

#endif // MATRIX4_H
