#include "Matrix4.h"

#include "Vector3.h"
#include "Quaternion.h"

Matrix4 Matrix4::identity = Matrix4();

Matrix4::Matrix4() : Matrix4(1.0f)
{
}

Matrix4::Matrix4(const glm::mat4 &m)
{
    c0 = Vector4(m[0][0], m[0][1], m[0][2], m[0][3]);
    c1 = Vector4(m[1][0], m[1][1], m[1][2], m[1][3]);
    c2 = Vector4(m[2][0], m[2][1], m[2][2], m[2][3]);
    c3 = Vector4(m[3][0], m[3][1], m[3][2], m[3][3]);
}

Matrix4::Matrix4(float a)
{
    c0 = Vector4(a, 0, 0, 0);
    c1 = Vector4(0, a, 0, 0);
    c2 = Vector4(0, 0, a, 0);
    c3 = Vector4(0, 0, 0, a);
}

Matrix4::Matrix4(const Vector4 &col0,
                 const Vector4 &col1,
                 const Vector4 &col2,
                 const Vector4 &col3)
{
    c0 = col0;
    c1 = col1;
    c2 = col2;
    c3 = col3;
}


Matrix4 Matrix4::Inversed() const
{
    Matrix4 inv;
    const Matrix4 &m = *this;

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


    float det = m[0].x * inv[0].x + m[0].y * inv[1].x + m[0].z * inv[2].x + m[0].w * inv[3].x;
    if (det == 0) return *this;
    return inv;
}

Matrix4 Matrix4::Transposed() const
{
    Matrix4 trans;
    const Matrix4 &m = *this;

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

float *Matrix4::GetFirstAddress() const
{
    return static_cast<float*>(&(c0.x));
}

void Matrix4::SetScale(const Vector3 &scale)
{
    c0.x = scale.x;
    c1.y = scale.y;
    c2.z = scale.z;
}

glm::mat4 Matrix4::ToGlmMat4() const
{
    glm::mat4 m;
    m[0] = glm::vec4(c0.x, c0.y, c0.z, c0.w);
    m[1] = glm::vec4(c1.x, c1.y, c1.z, c1.w);
    m[2] = glm::vec4(c2.x, c2.y, c2.z, c2.w);
    m[3] = glm::vec4(c3.x, c3.y, c3.z, c3.w);
    return m;
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

Vector4& Matrix4::operator[](int i) const
{
    if (i == 0) return c0;
    if (i == 1) return c1;
    if (i == 2) return c2;
    return c3;
}


Matrix4 operator*(const Matrix4 &m1, const Matrix4 &m2)
{
    Matrix4 m;
    m.c0 = Vector4((m1.c0.x * m2.c0.x) + (m1.c1.x * m2.c0.y) + (m1.c2.x * m2.c0.z) + (m1.c3.x * m2.c0.w),
                   (m1.c0.y * m2.c0.x) + (m1.c1.y * m2.c0.y) + (m1.c2.y * m2.c0.z) + (m1.c3.y * m2.c0.w),
                   (m1.c0.z * m2.c0.x) + (m1.c1.z * m2.c0.y) + (m1.c2.z * m2.c0.z) + (m1.c3.z * m2.c0.w),
                   (m1.c0.w * m2.c0.x) + (m1.c1.w * m2.c0.y) + (m1.c2.w * m2.c0.z) + (m1.c3.w * m2.c0.w));

    m.c1 = Vector4((m1.c0.x * m2.c1.x) + (m1.c1.x * m2.c1.y) + (m1.c2.x * m2.c1.z) + (m1.c3.x * m2.c1.w),
                   (m1.c0.y * m2.c1.x) + (m1.c1.y * m2.c1.y) + (m1.c2.y * m2.c1.z) + (m1.c3.y * m2.c1.w),
                   (m1.c0.z * m2.c1.x) + (m1.c1.z * m2.c1.y) + (m1.c2.z * m2.c1.z) + (m1.c3.z * m2.c1.w),
                   (m1.c0.w * m2.c1.x) + (m1.c1.w * m2.c1.y) + (m1.c2.w * m2.c1.z) + (m1.c3.w * m2.c1.w));

    m.c2 = Vector4((m1.c0.x * m2.c2.x) + (m1.c1.x * m2.c2.y) + (m1.c2.x * m2.c2.z) + (m1.c3.x * m2.c2.w),
                   (m1.c0.y * m2.c2.x) + (m1.c1.y * m2.c2.y) + (m1.c2.y * m2.c2.z) + (m1.c3.y * m2.c2.w),
                   (m1.c0.z * m2.c2.x) + (m1.c1.z * m2.c2.y) + (m1.c2.z * m2.c2.z) + (m1.c3.z * m2.c2.w),
                   (m1.c0.w * m2.c2.x) + (m1.c1.w * m2.c2.y) + (m1.c2.w * m2.c2.z) + (m1.c3.w * m2.c2.w));

    m.c3 = Vector4((m1.c0.x * m2.c3.x) + (m1.c1.x * m2.c3.y) + (m1.c2.x * m2.c3.z) + (m1.c3.x * m2.c3.w),
                   (m1.c0.y * m2.c3.x) + (m1.c1.y * m2.c3.y) + (m1.c2.y * m2.c3.z) + (m1.c3.y * m2.c3.w),
                   (m1.c0.z * m2.c3.x) + (m1.c1.z * m2.c3.y) + (m1.c2.z * m2.c3.z) + (m1.c3.z * m2.c3.w),
                   (m1.c0.w * m2.c3.x) + (m1.c1.w * m2.c3.y) + (m1.c2.w * m2.c3.z) + (m1.c3.w * m2.c3.w));
    return m;
}
