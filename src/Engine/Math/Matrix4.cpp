#include "Bang/Matrix4.h"

#include "Bang/Debug.h"
#include "Bang/String.h"
#include "Bang/Vector3.h"
#include "Bang/Quaternion.h"

Matrix4 Matrix4::Identity = Matrix4();

Matrix4::Matrix4() : Matrix4(1.0f)
{
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

Matrix4::Matrix4(float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33)
{
    c0 = Vector4(m00, m10, m20, m30);
    c1 = Vector4(m01, m11, m21, m31);
    c2 = Vector4(m02, m12, m22, m32);
    c3 = Vector4(m03, m13, m23, m33);
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


    float det = m[0].x * inv[0].x + m[0].y * inv[1].x + m[0].z * inv[2].x +
                m[0].w * inv[3].x;
    if (det == 0) return *this;

    inv.c0 *= 1.0f / det;
    inv.c1 *= 1.0f / det;
    inv.c2 *= 1.0f / det;
    inv.c3 *= 1.0f / det;

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

float *Matrix4::Data() const
{
    return SCAST<float*>(&(c0.x));
}

void Matrix4::SetTranslate(const Vector3 &translate)
{
    c3.x = translate.x;
    c3.y = translate.y;
    c3.z = translate.z;
}

void Matrix4::SetScale(const Vector3 &scale)
{
    c0.x = scale.x;
    c1.y = scale.y;
    c2.z = scale.z;
}

String Matrix4::ToString() const
{
    std::ostringstream oss;
    oss << "(" << c0[0] << ", " << c1[0] << ", " << c2[0] << ", " << c3[0] << "," << std::endl;
    oss << " " << c0[1] << ", " << c1[1] << ", " << c2[1] << ", " << c3[1] << "," << std::endl;
    oss << " " << c0[2] << ", " << c1[2] << ", " << c2[2] << ", " << c3[2] << "," << std::endl;
    oss << " " << c0[3] << ", " << c1[3] << ", " << c2[3] << ", " << c3[3] << ")" << std::endl;
    return String(oss.str());
}

Matrix4 Matrix4::LookAt(const Vector3 &eyePosition,
                        const Vector3 &focusPoint,
                        const Vector3 &up)
{
    const Vector3 f( (focusPoint - eyePosition).Normalized() );
    const Vector3 s( (Vector3::Cross(f, up)).Normalized() );
    const Vector3 u(  Vector3::Cross(s, f) );

    Matrix4 res(1.0f);
    res[0][0] =  s.x;
    res[1][0] =  s.y;
    res[2][0] =  s.z;
    res[0][1] =  u.x;
    res[1][1] =  u.y;
    res[2][1] =  u.z;
    res[0][2] = -f.x;
    res[1][2] = -f.y;
    res[2][2] = -f.z;
    res[3][0] = -Vector3::Dot(s, eyePosition);
    res[3][1] = -Vector3::Dot(u, eyePosition);
    res[3][2] =  Vector3::Dot(f, eyePosition);
    return res;
}

Matrix4 Matrix4::Perspective(float fovYRads,
                             float aspect,
                             float zNear,
                             float zFar)
{
    float const tanHalfFovy = Math::Tan(fovYRads / SCAST<float>(2));

    Matrix4 res(SCAST<float>(0));
    res[0][0] = SCAST<float>(1) / (aspect * tanHalfFovy);
    res[1][1] = SCAST<float>(1) / (tanHalfFovy);
    res[2][2] = -(zFar + zNear) / (zFar - zNear);
    res[2][3] = -SCAST<float>(1);
    res[3][2] = -(SCAST<float>(2) * zFar * zNear) / (zFar - zNear);

    return res;
}

Matrix4 Matrix4::Ortho(float left, float right,
                       float bottom, float top,
                       float zNear, float zFar)
{
    Matrix4 res(1);
    res[0][0] = SCAST<float>(2) / (right - left);
    res[1][1] = SCAST<float>(2) / (top - bottom);
    res[2][2] = -SCAST<float>(2) / (zFar - zNear);
    res[3][0] = -(right + left) / (right - left);
    res[3][1] = -(top + bottom) / (top - bottom);
    res[3][2] = -(zFar + zNear) / (zFar - zNear);
    return res;
}

Matrix4 Matrix4::TranslateMatrix(const Vector3 &v)
{
    return Matrix4(1, 0, 0, v.x,
                   0, 1, 0, v.y,
                   0, 0, 1, v.z,
                   0, 0, 0, 1);
}

Matrix4 Matrix4::RotateMatrix(const Quaternion &q)
{
    float qxx(q.x * q.x);
    float qyy(q.y * q.y);
    float qzz(q.z * q.z);
    float qxz(q.x * q.z);
    float qxy(q.x * q.y);
    float qyz(q.y * q.z);
    float qwx(q.w * q.x);
    float qwy(q.w * q.y);
    float qwz(q.w * q.z);

    Matrix4 res(SCAST<float>(1));

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

Matrix4 Matrix4::ScaleMatrix(const Vector3 &v)
{
    return Matrix4(v.x,   0,   0,  0,
                     0, v.y,   0,  0,
                     0,   0, v.z,  0,
                     0,   0,   0,  1);
}

Vector4& Matrix4::operator[](int i) const
{
    switch (i)
    {
        case 0: return c0;
        case 1: return c1;
        case 2: return c2;
        case 3: return c3;
    }
    Debug_Warn("Matrix4 index " << i << " too big");
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
