#include "Matrix4.h"

#include "Vector3.h"
#include "Quaternion.h"


Matrix4 Matrix4::identity = Matrix4();

Matrix4::Matrix4()
{
}

Matrix4::Matrix4(const glm::mat4 &m) : glm::mat4(m)
{
}

Matrix4::Matrix4(float a) : glm::mat4(a)
{
}

Matrix4 Matrix4::Inversed() const
{
    return Matrix4(glm::inverse(glm::mat4(*this)));
}

Matrix4 Matrix4::Transposed() const
{
    return Matrix4(glm::transpose(glm::mat4(*this)));
}

float *Matrix4::GetFirstAddress() const
{
    return (float*)(&(this[0][0]));
}

Matrix4 Matrix4::Perspective(float fovY, float aspect, float zNear, float zFar)
{
    return Matrix4(glm::perspective(fovY, aspect, zNear, zFar));
}

Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    return Matrix4(glm::ortho(left, right, bottom, top, zNear, zFar));
}

Matrix4 Matrix4::TranslateMatrix(const Vector3 &v)
{
    return Matrix4(glm::translate(glm::mat4(1.0f), v));
}

Matrix4 Matrix4::RotateMatrix(const Quaternion &q)
{
    return Matrix4(glm::mat4_cast(q));
}

Matrix4 Matrix4::ScaleMatrix(const Vector3 &v)
{
    return Matrix4(glm::scale(glm::mat4(1.0f), v));
}


Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2)
{
    return Matrix4(glm::mat4(m1) * glm::mat4(m2));
}


