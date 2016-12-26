#include "Transform.h"

#include <sstream>
#include <ostream>

#include "Debug.h"
#include "GameObject.h"

Transform::Transform()
{
}

void Transform::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Transform *t = static_cast<Transform*>(clone);
    t->SetLocalPosition(GetLocalPosition());
    t->SetLocalRotation(GetLocalRotation());
    t->SetLocalScale(GetLocalScale());
}


ICloneable *Transform::Clone() const
{
    Transform *t = new Transform();
    CloneInto(t);
    return t;
}

Transform::~Transform()
{
}

void Transform::SetLocalPosition(const Vector3 &p)
{
    m_localPosition = p;
    m_hasChanged = true;
}
void Transform::SetPosition(const Vector3 &p)
{
    if (!gameObject->parent) SetLocalPosition(p);
    else
    {
        SetLocalPosition(gameObject->parent->transform->WorldToLocalPoint(p));
    }
}
void Transform::TranslateLocal(const Vector3 &translation)
{
    SetLocalPosition(GetLocalPosition() + translation);
}
void Transform::Translate(const Vector3 &translation)
{
    SetPosition(GetPosition() + translation);
}



void Transform::SetLocalRotation(const Quaternion &q)
{
    m_localRotation = q.Normalized();
    m_localEuler = Quaternion::EulerAngles(m_localRotation).ToDegrees();
    m_hasChanged = true;
}
void Transform::SetLocalEuler(const Vector3 &degreesEuler)
{
    Vector3 eulers = degreesEuler;
    eulers.x = std::fmod(eulers.x, 360.0f);
    eulers.y = std::fmod(eulers.y, 360.0f);
    eulers.z = std::fmod(eulers.z, 360.0f);

    m_localEuler = eulers;

    Vector3 rads = m_localEuler.ToRadians();
    Quaternion qx = Quaternion::AngleAxis(rads.x, Vector3::Right);
    Quaternion qy = Quaternion::AngleAxis(rads.y, Vector3::Up);
    Quaternion qz = Quaternion::AngleAxis(rads.z, Vector3::Forward);
    m_localRotation = (qz * qy * qx).Normalized();
    m_hasChanged = true;
}
void Transform::SetLocalEuler(float x, float y, float z)
{
    SetLocalEuler(Vector3(x,y,z));
}

void Transform::SetRotation(const Quaternion &q)
{
    if (!gameObject->parent) SetLocalRotation(q.Normalized());
    else
    {
        SetLocalRotation(Quaternion(-gameObject->parent->transform->GetRotation() * q.Normalized()));
    }
}
void Transform::SetEuler(const Vector3 &degreesEuler)
{
    if (!gameObject->parent) SetLocalEuler(degreesEuler);
    else
    {
        SetLocalEuler(-gameObject->parent->transform->GetEuler() + degreesEuler);
    }
}
void Transform::SetEuler(float x, float y, float z)
{
    SetEuler(Vector3(x,y,z));
}

void Transform::RotateLocal(const Quaternion &r)
{
    SetLocalRotation(Quaternion(GetLocalRotation() * r.Normalized()));
}
void Transform::RotateLocalEuler(const Vector3 &degreesEuler)
{
    SetLocalEuler(GetLocalEuler() + degreesEuler);
}
void Transform::Rotate(const Quaternion &r)
{
    SetRotation(Quaternion(r.Normalized() * GetRotation()));
}
void Transform::RotateEuler(const Vector3 &degreesEuler)
{
    SetEuler(GetEuler() + degreesEuler);
}



void Transform::SetScale(float s)
{
    SetScale(Vector3(s));
}

void Transform::SetScale(const Vector3 &v)
{
    SetLocalScale(1.0f / gameObject->parent->transform->GetScale() * v);
}

void Transform::SetLocalScale(float s)
{
    SetLocalScale(Vector3(s));
}

void Transform::SetLocalScale(const Vector3 &s)
{
    m_localScale = s;
    m_hasChanged = true;
}



Vector3 Transform::TransformPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalToWorldMatrix(&m);
    return Vector3((m * Vector4(point, 1)).xyz());
}
Vector3 Transform::InverseTransformPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalToWorldMatrix(&m);
    return Vector3((m.Inversed() * Vector4(point, 1)).xyz());
}
Vector3 Transform::TransformDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalToWorldMatrix(&m);
    return Vector3((m * Vector4(dir, 0)).xyz());
}
Vector3 Transform::InverseTransformDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalToWorldMatrix(&m);
    return Vector3((m.Inversed() * Vector4(dir, 0)).xyz());
}


Vector3 Transform::ParentToLocalPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalToParentMatrix(&m);
    return Vector3((m.Inversed() * Vector4(point, 1)).xyz());
}
Vector3 Transform::ParentToLocalDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalToParentMatrix(&m);
    return Vector3((m.Inversed() * Vector4(dir, 0)).xyz());
}
Vector3 Transform::LocalToParentPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalToParentMatrix(&m);
    return Vector3((m * Vector4(point, 1)).xyz());
}
Vector3 Transform::LocalToParentDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalToParentMatrix(&m);
    return Vector3((m * Vector4(dir, 0)).xyz());
}


Vector3 Transform::LocalToWorldPoint(const Vector3 &point) const
{
    return TransformPoint(point);
}
Vector3 Transform::LocalToWorldDirection(const Vector3 &dir) const
{
    return TransformDirection(dir);
}
Vector3 Transform::WorldToLocalPoint(const Vector3 &point) const
{
    return InverseTransformPoint(point);
}
Vector3 Transform::WorldToLocalDirection(const Vector3 &dir) const
{
    return InverseTransformDirection(dir);
}

const Matrix4 &Transform::GetLocalToParentMatrix() const
{
    if (m_hasChanged || true)
    {
        Matrix4 T = Matrix4::TranslateMatrix(GetLocalPosition());
        Matrix4 R = Matrix4::RotateMatrix(GetLocalRotation());
        Matrix4 S = Matrix4::ScaleMatrix(GetLocalScale());

        m_localToWorldMatrix = T * R * S;
        m_hasChanged = false;
    }
    return m_localToWorldMatrix;
}

void Transform::GetLocalToParentMatrix(Matrix4 *m) const
{
    *m = GetLocalToParentMatrix();
}

void Transform::GetLocalToWorldMatrix(Matrix4 *m) const
{
    GetLocalToParentMatrix(m);
    if (gameObject->parent)
    {
        Matrix4 mp;
        gameObject->parent->transform->GetLocalToWorldMatrix(&mp);
        *m = mp * (*m);
    }
}

void Transform::GetLocalToWorldNormalMatrix(Matrix4 *m) const
{
    GetLocalToWorldMatrix(m);
    *m = m->Inversed().Transposed();
}


void Transform::LookAt(const Vector3 &target, const Vector3 &_up)
{
    Assert(target != m_localPosition, "LookAt target is the same as position.", return);
    Vector3 up = _up.Normalized();
    SetRotation(Quaternion::LookDirection(target - GetPosition(), up) );
}

void Transform::LookInDirection(const Vector3 &dir, const Vector3 &up)
{
    LookAt(GetPosition() + dir * 99, up);
}

Vector3 Transform::GetLocalPosition() const
{
    return m_localPosition;
}

Vector3 Transform::GetPosition() const
{
    if (!gameObject->parent)
    {
        return GetLocalPosition();
    }
    else
    {
        return gameObject->parent->transform->LocalToWorldPoint(GetLocalPosition());
    }
}

Quaternion Transform::GetLocalRotation() const
{
    return m_localRotation;
}

Quaternion Transform::GetRotation() const
{
    if (!gameObject->parent)
    {
        return GetLocalRotation();
    }
    else
    {
        return gameObject->parent->transform->GetRotation() * GetLocalRotation();
    }
}

Vector3 Transform::GetLocalEuler() const
{
    return m_localEuler;
}

Vector3 Transform::GetEuler() const
{
    if (!gameObject->parent)
    {
        return GetLocalEuler();
    }
    else
    {
        return gameObject->parent->transform->GetEuler() + GetLocalEuler();
    }
}

Vector3 Transform::GetLocalScale() const
{
    return m_localScale;
}

Vector3 Transform::GetScale() const
{
    if (!gameObject->parent)
    {
        return GetLocalScale();
    }
    else
    {
        return gameObject->parent->transform->GetScale() * GetLocalScale();
    }
}

Vector3 Transform::GetPositionFromMatrix4(const Matrix4 &tm)
{
    return tm.c3.xyz();
}

Quaternion Transform::GetRotationFromMatrix4(const Matrix4 &tm)
{
    Vector3 scale = Transform::GetScaleFromMatrix4(tm);

    Matrix4 rotMatrix;
    rotMatrix.c0 = Vector4(tm.c0.xyz() / scale.x, 0);
    rotMatrix.c1 = Vector4(tm.c1.xyz() / scale.y, 0);
    rotMatrix.c2 = Vector4(tm.c2.xyz() / scale.z, 0);
    rotMatrix.c3 = Vector4(0, 0, 0, 1);

    return Quaternion::FromRotationMatrix(rotMatrix);
}

Vector3 Transform::GetScaleFromMatrix4(const Matrix4 &tm)
{
    return Vector3(tm.c0.Length(), tm.c1.Length(), tm.c2.Length());
}

Transform Transform::FromTransformMatrix(const Matrix4 &transformMatrix)
{
    Transform t;
    t.SetLocalPosition(Transform::GetPositionFromMatrix4(transformMatrix));
    t.SetLocalRotation(Transform::GetRotationFromMatrix4(transformMatrix));
    t.SetLocalScale   (Transform::GetScaleFromMatrix4(transformMatrix));
    return t;
}

Vector3 Transform::GetForward() const
{
    return LocalToWorldDirection(Vector3::Forward).Normalized();
}

Vector3 Transform::GetBack() const
{
    return -GetForward();
}

Vector3 Transform::GetRight() const
{
    return  LocalToWorldDirection(Vector3::Right).Normalized();
}

Vector3 Transform::GetLeft() const
{
    return -GetRight();
}

Vector3 Transform::GetUp() const
{
    return LocalToWorldDirection(Vector3::Up).Normalized();
}

Vector3 Transform::GetDown() const
{
    return -GetUp();
}


const String Transform::ToString() const
{
    using std::operator<<;

    std::ostringstream msg;

    Vector3 euler = Quaternion::EulerAngles(m_localRotation);
    msg << "[" <<
           "Transform: " << std::endl <<
           "   Position: (" << m_localPosition.x << ", " << m_localPosition.y << ", " << m_localPosition.z << ")"  << std::endl <<
           "   Rotation: (" << glm::degrees(euler.x) << ", " << glm::degrees(euler.y) << ", " << glm::degrees(euler.z) << ")" << std::endl <<
           "   Scale: (" << m_localScale.x << ", " << m_localScale.y << ", " << m_localScale.z << ")" << std::endl <<
           "]" << std::endl;

    return msg.str();
}

#ifdef BANG_EDITOR

void Transform::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void Transform::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

#endif

void Transform::ReadXMLInfo(const XMLNode *xmlInfo)
{
    Component::ReadXMLInfo(xmlInfo);
    SetLocalPosition(xmlInfo->GetVector3("Position"));
    SetLocalEuler(xmlInfo->GetVector3("Rotation"));
    SetLocalScale(xmlInfo->GetVector3("Scale"));
}

void Transform::FillXMLInfo(XMLNode *xmlInfo) const
{
    Component::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName("Transform");

    xmlInfo->SetVector3("Position", GetLocalPosition());
    xmlInfo->SetVector3("Rotation", GetLocalEuler());
    xmlInfo->SetVector3("Scale",    GetLocalScale());
}
