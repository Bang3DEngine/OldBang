#include "Bang/Transform.h"

#include <sstream>
#include <ostream>

#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/ITransformListener.h"

USING_NAMESPACE_BANG

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::SetLocalPosition(const Vector3 &p)
{
    if (GetLocalPosition() != p)
    {
        m_localPosition = p;
        Invalidate();
    }
}
void Transform::SetPosition(const Vector3 &p)
{
    if (!GetGameObject()->GetParent()) SetLocalPosition(p);
    else
    {
        SetLocalPosition(GetGameObject()->GetParent()->
                         GetTransform()->WorldToLocalPoint(p));
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
    if (GetLocalRotation() != q)
    {
        m_localRotation = q.Normalized();
        Invalidate();
    }
}
void Transform::SetLocalEuler(const Vector3 &degreesEuler)
{
    Vector3 eulers = degreesEuler;
    eulers.x = std::fmod(eulers.x, 360.0f);
    eulers.y = std::fmod(eulers.y, 360.0f);
    eulers.z = std::fmod(eulers.z, 360.0f);

    Vector3 rads = eulers.ToRadians();
    Quaternion qx = Quaternion::AngleAxis(rads.x, Vector3::Right);
    Quaternion qy = Quaternion::AngleAxis(rads.y, Vector3::Up);
    Quaternion qz = Quaternion::AngleAxis(rads.z, Vector3::Forward);
    SetLocalRotation( (qz * qy * qx).Normalized() );
}
void Transform::SetLocalEuler(float x, float y, float z)
{
    SetLocalEuler(Vector3(x,y,z));
}

void Transform::SetRotation(const Quaternion &q)
{
    if (!GetGameObject()->GetParent()) SetLocalRotation(q.Normalized());
    else
    {
        SetLocalRotation(
            Quaternion(-GetGameObject()->GetParent()->GetTransform()->
                       GetRotation() * q.Normalized()));
    }
}
void Transform::SetEuler(const Vector3 &degreesEuler)
{
    if (!GetGameObject()->GetParent()) SetLocalEuler(degreesEuler);
    else
    {
        SetLocalEuler(-GetGameObject()->GetParent()->GetTransform()->
                      GetEuler() + degreesEuler);
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
    GameObject *p = GetGameObject()->GetParent();
    Vector3 parentScale = p ? p->GetTransform()->GetScale() : Vector3::One;
    parentScale = Vector3::Max( Vector3(0.0001f), parentScale );
    SetLocalScale(1.0f / parentScale * v);
}

void Transform::SetLocalScale(float s)
{
    SetLocalScale(Vector3(s));
}

void Transform::SetLocalScale(const Vector3 &s)
{
    if (GetLocalScale() != s)
    {
        m_localScale = s;
        Invalidate();
    }
}

Vector3 Transform::TransformPoint(const Vector3 &point) const
{
    return (GetLocalToWorldMatrix() * Vector4(point, 1)).xyz();
}
Vector3 Transform::InverseTransformPoint(const Vector3 &point) const
{
    return (GetLocalToWorldMatrix().Inversed() * Vector4(point, 1)).xyz();
}
Vector3 Transform::TransformDirection(const Vector3 &dir) const
{
    return GetRotation() * dir;
}
Vector3 Transform::InverseTransformDirection(const Vector3 &dir) const
{
    return GetRotation().Inversed() * dir;
}
Vector3 Transform::TransformVector(const Vector3 &dir) const
{
    return (GetLocalToWorldMatrix() * Vector4(dir, 0)).xyz();
}
Vector3 Transform::InverseTransformVector(const Vector3 &dir) const
{
    return (GetLocalToWorldMatrix().Inversed() * Vector4(dir, 0)).xyz();
}


Vector3 Transform::ParentToLocalPoint(const Vector3 &point) const
{
    return (GetLocalToWorldMatrix().Inversed() * Vector4(point, 1)).xyz();
}

Vector3 Transform::ParentToLocalVector(const Vector3 &vector) const
{
    return (GetLocalToParentMatrix().Inversed() * Vector4(vector, 0)).xyz();
}
Vector3 Transform::ParentToLocalDirection(const Vector3 &dir) const
{
    return GetLocalRotation().Inversed() * dir;
}
Vector3 Transform::LocalToParentPoint(const Vector3 &point) const
{
    return (GetLocalToParentMatrix() * Vector4(point, 1)).xyz();
}

Vector3 Transform::LocalToParentVector(const Vector3 &vector) const
{
    return (GetLocalToParentMatrix() * Vector4(vector, 0)).xyz();
}
Vector3 Transform::LocalToParentDirection(const Vector3 &dir) const
{
    return GetLocalRotation() * dir;
}


Vector3 Transform::LocalToWorldPoint(const Vector3 &point) const
{
    return TransformPoint(point);
}
Vector3 Transform::LocalToWorldVector(const Vector3 &dir) const
{
    return TransformVector(dir);
}
Vector3 Transform::LocalToWorldDirection(const Vector3 &dir) const
{
    return TransformDirection(dir);
}

Vector3 Transform::WorldToLocalPoint(const Vector3 &point) const
{
    return InverseTransformPoint(point);
}
Vector3 Transform::WorldToLocalVector(const Vector3 &dir) const
{
    return InverseTransformVector(dir);
}

Vector3 Transform::WorldToLocalDirection(const Vector3 &dir) const
{
    return InverseTransformDirection(dir);
}

const Matrix4 &Transform::GetLocalToParentMatrix() const
{
    if (!IsInvalid()) { return m_localToParentMatrix; }

    Matrix4 T  = Matrix4::TranslateMatrix(GetLocalPosition());
    Matrix4 R  = Matrix4::RotateMatrix(GetLocalRotation());
    Matrix4 S  = Matrix4::ScaleMatrix(GetLocalScale());

    m_localToParentMatrix = (T * R * S);
    Validate();

    return m_localToParentMatrix;
}

bool Transform::CanBeRepeatedInGameObject() const { return false; }

void Transform::RecalculateLocalToWorldMatrix() const
{
    const Matrix4 &m = GetLocalToParentMatrix();
    if (GetGameObject()->GetParent() &&
        GetGameObject()->GetParent()->GetTransform())
    {
        const Matrix4 &mp =
          GetGameObject()->GetParent()->GetTransform()->GetLocalToWorldMatrix();
        m_localToWorldMatrix = mp * m;
    }
    else { m_localToWorldMatrix = m; }

    m_invalidLocalToWorldMatrix = false;
}

const Matrix4& Transform::GetLocalToWorldMatrix() const
{
    if (m_invalidLocalToWorldMatrix) { RecalculateLocalToWorldMatrix(); }
    return m_localToWorldMatrix;
}

void Transform::LookAt(const Vector3 &target, const Vector3 &_up)
{
    if(target == GetLocalPosition()) { return; }
    Vector3 up = _up.Normalized();
    SetRotation(Quaternion::LookDirection(target - GetPosition(), up) );
}

void Transform::LookAt(Transform *targetTransform, const Vector3 &up)
{
    LookAt(targetTransform->GetPosition(), up);
}

void Transform::LookAt(GameObject *target, const Vector3 &up)
{
    LookAt(target->GetTransform(), up);
}

void Transform::LookInDirection(const Vector3 &dir, const Vector3 &up)
{
    LookAt(GetPosition() + dir * 99.0f, up);
}

const Vector3& Transform::GetLocalPosition() const
{
    return m_localPosition;
}

Vector3 Transform::GetPosition() const
{
    if (!GetGameObject()->GetParent() ||
        !GetGameObject()->GetParent()->GetTransform())
    {
        return GetLocalPosition();
    }
    else
    {
        return GetGameObject()->GetParent()->GetTransform()->
               LocalToWorldPoint(GetLocalPosition());
    }
}

const Quaternion& Transform::GetLocalRotation() const
{
    return m_localRotation;
}

Quaternion Transform::GetRotation() const
{
    if (!GetGameObject()->GetParent() ||
        !GetGameObject()->GetParent()->GetTransform())
    {
        return GetLocalRotation();
    }
    else
    {
        return GetGameObject()->GetParent()->GetTransform()->
               GetRotation() * GetLocalRotation();
    }
}

Vector3 Transform::GetLocalEuler() const
{
    return GetLocalRotation().GetEulerAngles();
}

Vector3 Transform::GetEuler() const
{
    return LocalToWorldDirection( GetLocalEuler() );
}

const Vector3& Transform::GetLocalScale() const
{
    return m_localScale;
}

Vector3 Transform::GetScale() const
{
    return LocalToWorldVector(GetLocalScale());
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

    return Matrix4::ToQuaternion(rotMatrix);
}

Vector3 Transform::GetScaleFromMatrix4(const Matrix4 &tm)
{
    return Vector3(tm.c0.Length(), tm.c1.Length(), tm.c2.Length());
}

Vector3 Transform::GetForward() const
{
    return LocalToWorldVector(Vector3::Forward).Normalized();
}

Vector3 Transform::GetBack() const
{
    return -GetForward();
}

Vector3 Transform::GetRight() const
{
    return  LocalToWorldVector(Vector3::Right).Normalized();
}

Vector3 Transform::GetLeft() const
{
    return -GetRight();
}

Vector3 Transform::GetUp() const
{
    return LocalToWorldVector(Vector3::Up).Normalized();
}

Vector3 Transform::GetDown() const
{
    return -GetUp();
}

void Transform::OnInvalidated()
{
    OnTransformChanged();
    m_invalidLocalToWorldMatrix = true;
}

void Transform::OnTransformChanged()
{
    IInvalidatable<Transform>::Invalidate();

    if (!GetGameObject()) { return; }

    ITransformListener::SetReceiveEvents(false);
    EventEmitter<ITransformListener>::
       PropagateToListeners(&ITransformListener::OnTransformChanged);
    GetGameObject()->Propagate(&ITransformListener::OnTransformChanged,
                               GetGameObject()->GetComponents<ITransformListener>());
    ITransformListener::SetReceiveEvents(true);

    PropagateParentTransformChangedEvent();
    PropagateChildrenTransformChangedEvent();
}

void Transform::PropagateParentTransformChangedEvent() const
{
    EventEmitter<ITransformListener>::
           PropagateToListeners(&ITransformListener::OnParentTransformChanged);
    GetGameObject()->Propagate(&ITransformListener::OnParentTransformChanged,
        GetGameObject()->GetComponentsInChildrenOnly<ITransformListener>(false));
}

void Transform::PropagateChildrenTransformChangedEvent() const
{
    GameObject *go = GetGameObject();
    EventEmitter<ITransformListener>::
           PropagateToListeners(&ITransformListener::OnChildrenTransformChanged);
    GetGameObject()->Propagate(&ITransformListener::OnChildrenTransformChanged,
                          go->GetComponentsInParent<ITransformListener>(false));
}

void Transform::OnParentTransformChanged()
{
    IInvalidatable<Transform>::Invalidate();
}

void Transform::OnChildrenTransformChanged()
{
    PropagateChildrenTransformChangedEvent();
}


void Transform::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Transform *t = Cast<Transform*>(clone);
    t->SetLocalPosition(GetLocalPosition());
    t->SetLocalRotation(GetLocalRotation());
    t->SetLocalScale(GetLocalScale());
}

void Transform::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Position"))
    { SetLocalPosition(xmlInfo.Get<Vector3>("Position")); }

    if (xmlInfo.Contains("Rotation"))
    { SetLocalRotation(xmlInfo.Get<Quaternion>("Rotation")); }

    if (xmlInfo.Contains("Scale"))
    { SetLocalScale(xmlInfo.Get<Vector3>("Scale")); }
}

void Transform::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
    xmlInfo->Set("Position", GetLocalPosition());
    xmlInfo->Set("Rotation", GetLocalRotation());
    xmlInfo->Set("Scale",    GetLocalScale());
}

