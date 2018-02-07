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
                         GetTransform()->FromWorldToLocalPoint(p));
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
    Vector3 eulersRads = eulers.ToRadians();
    SetLocalRotation( Quaternion::FromEulerAngles(eulersRads).Normalized() );
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
            Quaternion(GetGameObject()->GetParent()->GetTransform()->
                       GetRotation().Inversed() * q.Normalized()));
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
    return (GetLocalToWorldMatrixInv() * Vector4(point, 1)).xyz();
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
    return (GetLocalToWorldMatrixInv() * Vector4(dir, 0)).xyz();
}

Vector3 Transform::FromLocalToWorldPoint(const Vector3 &point) const
{
    return TransformPoint(point);
}
Vector3 Transform::FromLocalToWorldVector(const Vector3 &dir) const
{
    return TransformVector(dir);
}
Vector3 Transform::FromLocalToWorldDirection(const Vector3 &dir) const
{
    return TransformDirection(dir);
}

Vector3 Transform::FromWorldToLocalPoint(const Vector3 &point) const
{
    return InverseTransformPoint(point);
}
Vector3 Transform::FromWorldToLocalVector(const Vector3 &dir) const
{
    return InverseTransformVector(dir);
}

Vector3 Transform::FromWorldToLocalDirection(const Vector3 &dir) const
{
    return InverseTransformDirection(dir);
}

bool Transform::CanBeRepeatedInGameObject() const { return false; }

void Transform::RecalculateParentMatricesIfNeeded() const
{
    if (IsInvalid())
    {
        CalculateLocalToParentMatrix();
        Validate();
    }
}

void Transform::RecalculateWorldMatricesIfNeeded() const
{
    if (m_invalidLocalToWorldMatrix)
    {
        CalculateLocalToWorldMatrix();
        m_invalidLocalToWorldMatrix = false;
    }
}

void Transform::CalculateLocalToParentMatrix() const
{
    Matrix4 T  = Matrix4::TranslateMatrix(GetLocalPosition());
    Matrix4 R  = Matrix4::RotateMatrix(GetLocalRotation());
    Matrix4 S  = Matrix4::ScaleMatrix(GetLocalScale());

    m_localToParentMatrix = (T * R * S);
    m_localToParentMatrixInv = m_localToParentMatrix.Inversed();
}

void Transform::CalculateLocalToWorldMatrix() const
{
    const Matrix4 &m = GetLocalToParentMatrix();
    if (GetGameObject()->GetParent() &&
        GetGameObject()->GetParent()->GetTransform())
    {
        const Matrix4 &mp =
          GetGameObject()->GetParent()->GetTransform()->GetLocalToWorldMatrix();
        m_localToWorldMatrix = mp * m;
    }
    else
    {
        m_localToWorldMatrix = m;
    }
    m_localToWorldMatrixInv = m_localToWorldMatrix.Inversed();
}

const Matrix4 &Transform::GetLocalToParentMatrix() const
{
    RecalculateParentMatricesIfNeeded();
    return m_localToParentMatrix;
}

const Matrix4 &Transform::GetLocalToParentMatrixInv() const
{
    RecalculateParentMatricesIfNeeded();
    return m_localToParentMatrixInv;
}

const Matrix4& Transform::GetLocalToWorldMatrix() const
{
    RecalculateWorldMatricesIfNeeded();
    return m_localToWorldMatrix;
}

const Matrix4 &Transform::GetLocalToWorldMatrixInv() const
{
    RecalculateWorldMatricesIfNeeded();
    return m_localToWorldMatrixInv;
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
               FromLocalToWorldPoint(GetLocalPosition());
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
    return GetLocalRotation().GetEulerAngles().ToDegrees();
}

Vector3 Transform::GetEuler() const
{
    return FromLocalToWorldDirection( GetLocalEuler() );
}

const Vector3& Transform::GetLocalScale() const
{
    return m_localScale;
}

Vector3 Transform::GetScale() const
{
    return FromLocalToWorldVector(GetLocalScale());
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
    return FromLocalToWorldVector(Vector3::Forward).Normalized();
}

Vector3 Transform::GetBack() const
{
    return -GetForward();
}

Vector3 Transform::GetRight() const
{
    return  FromLocalToWorldVector(Vector3::Right).Normalized();
}

Vector3 Transform::GetLeft() const
{
    return -GetRight();
}

Vector3 Transform::GetUp() const
{
    return FromLocalToWorldVector(Vector3::Up).Normalized();
}

Vector3 Transform::GetDown() const
{
    return -GetUp();
}

void Transform::OnInvalidated()
{
    m_invalidLocalToWorldMatrix = true;
    OnTransformChanged();
}

void Transform::OnParentChanged(GameObject*, GameObject*)
{
    OnParentTransformChanged();
}

void Transform::OnTransformChanged()
{
    IInvalidatable<Transform>::Invalidate();

    GameObject *go = GetGameObject();
    if (!go) { return; }

    EventEmitter<ITransformListener>::
       PropagateToListeners(&ITransformListener::OnTransformChanged);

    ITransformListener::SetReceiveEvents(false);

    go->Propagate(&ITransformListener::OnTransformChanged,
                  go->GetComponents<ITransformListener>());

    ITransformListener::SetReceiveEvents(true);

    PropagateParentTransformChangedEventToChildren();
    PropagateChildrenTransformChangedEventToParent();
}

void Transform::PropagateParentTransformChangedEventToChildren() const
{
    GameObject *go = GetGameObject();
    EventEmitter<ITransformListener>::
           PropagateToListeners(&ITransformListener::OnParentTransformChanged);
    go->Propagate(&ITransformListener::OnParentTransformChanged,
                  go->GetComponentsInChildrenOnly<ITransformListener>(false));
}

void Transform::PropagateChildrenTransformChangedEventToParent() const
{
    GameObject *go = GetGameObject();
    EventEmitter<ITransformListener>::
           PropagateToListeners(&ITransformListener::OnChildrenTransformChanged);
    go->Propagate(&ITransformListener::OnChildrenTransformChanged,
                  go->GetComponentsInParent<ITransformListener>(false));
}

void Transform::OnParentTransformChanged()
{
    IInvalidatable<Transform>::Invalidate();
    PropagateParentTransformChangedEventToChildren();
}

void Transform::OnChildrenTransformChanged()
{
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

