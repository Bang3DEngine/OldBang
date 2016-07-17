#include "Transform.h"
#include "FileReader.h"
#include "GameObject.h"

Transform::Transform() : m_localPosition(Vector3(0.0f))
                        ,m_localRotation(Quaternion())
                        ,m_localScale(Vector3(1.0f))
                        #ifdef BANG_EDITOR
                        ,m_localEuler(Vector3(0.0f))
                        #endif
{
    #ifdef BANG_EDITOR
    m_inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo(
            "Position", {m_localPosition.x, m_localPosition.y, m_localPosition.z}
        ),
        new InspectorVFloatSWInfo(
            "Rotation", {m_localEuler.x, m_localEuler.y, m_localEuler.z}
        ),
        new InspectorVFloatSWInfo(
            "Scale", {m_localScale.x, m_localScale.y, m_localScale.z}
        )
    });
    #endif
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
}
void Transform::SetPosition(const Vector3 &p)
{
    Vector3 localPos = p;
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) localPos = pt->WorldToLocalPoint(p);
    }
    SetLocalPosition(localPos);
    /*
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) SetLocalPosition(-pt->GetPosition() + localPos);
        else SetLocalPosition(localPos);
    }
    else SetLocalPosition(localPos);*/
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
}
void Transform::SetLocalEuler(const Vector3 &degreesEuler)
{
    m_localEuler = degreesEuler;
    Vector3 rads = degreesEuler.ToRadians();
    Quaternion qx = Quaternion::AngleAxis(rads.x, Vector3::right);
    Quaternion qy = Quaternion::AngleAxis(rads.y, Vector3::up);
    Quaternion qz = Quaternion::AngleAxis(rads.z, Vector3::forward);
    m_localRotation = (qz * qy * qx).Normalized();
}
void Transform::SetLocalEuler(float x, float y, float z)
{
    SetLocalEuler(Vector3(x,y,z));
}

void Transform::SetRotation(const Quaternion &q)
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) SetLocalRotation(Quaternion(-pt->GetRotation() * q.Normalized()));
        else SetLocalRotation(q);
    }
    else SetLocalRotation(q);
}
void Transform::SetEuler(const Vector3 &degreesEuler)
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) SetLocalEuler(-pt->GetEuler() + degreesEuler);
        else SetLocalEuler(degreesEuler);
    }
    else SetLocalEuler(degreesEuler);
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

#ifdef BANG_EDITOR
void Transform::SetLocalRotationFromInspector(const Quaternion &q)
{
    m_localRotation = q.Normalized();
}
#endif




void Transform::SetScale(float s)
{
    SetScale(Vector3(s));
}

void Transform::SetScale(const Vector3 &v)
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) SetLocalScale(1.0f / pt->GetScale() * v);
        else SetLocalScale(v);
    }
    else SetLocalScale(v);
}

void Transform::SetLocalScale(float s)
{
    SetLocalScale(Vector3(s));
}

void Transform::SetLocalScale(const Vector3 &s)
{
    m_localScale = s;
}

Vector3 Transform::TransformDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetModelMatrix(m);
    return Vector3((m * glm::vec4(dir, 0)).xyz());
}

Vector3 Transform::InverseTransformDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetModelMatrix(m);
    return Vector3((m.Inversed() * glm::vec4(dir, 0)).xyz());
}

Vector3 Transform::TransformPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetModelMatrix(m);
    return Vector3((m * glm::vec4(point, 1)).xyz());
}

Vector3 Transform::InverseTransformPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetModelMatrix(m);
    return Vector3((m.Inversed() * glm::vec4(point, 1)).xyz());
}

Vector3 Transform::LocalToWorldDirection(const Vector3 &dir) const
{
    return TransformDirection(dir);
}

Vector3 Transform::LocalToParentDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalModelMatrix(m);
    return Vector3((m * glm::vec4(dir, 0)).xyz());
}

Vector3 Transform::ParentToLocalDirection(const Vector3 &dir) const
{
    Matrix4 m;
    GetLocalModelMatrix(m);
    return Vector3((m.Inversed() * glm::vec4(dir, 0)).xyz());
}

Vector3 Transform::WorldToLocalDirection(const Vector3 &dir) const
{
    return InverseTransformDirection(dir);
}

Vector3 Transform::LocalToWorldPoint(const Vector3 &point) const
{
    return TransformPoint(point);
}

Vector3 Transform::ParentToLocalPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalModelMatrix(m);
    return Vector3((m.Inversed() * glm::vec4(point, 1)).xyz());
}

Vector3 Transform::LocalToParentPoint(const Vector3 &point) const
{
    Matrix4 m;
    GetLocalModelMatrix(m);
    return Vector3((m * glm::vec4(point, 1)).xyz());
}

Vector3 Transform::WorldToLocalPoint(const Vector3 &point) const
{
    return InverseTransformPoint(point);
}

void Transform::GetLocalModelMatrix(Matrix4 &m) const
{
    Matrix4 T = Matrix4::TranslateMatrix(GetLocalPosition());
    Matrix4 R = Matrix4::RotateMatrix(GetLocalRotation());
    Matrix4 S = Matrix4::ScaleMatrix(GetLocalScale());

    m = T * R * S;
}

void Transform::GetModelMatrix(Matrix4 &m) const
{
    GetLocalModelMatrix(m);

    if (gameObject->parent)
    {
        Transform *tp = gameObject->parent->transform;
        if (tp)
        {
            Matrix4 mp;
            tp->GetModelMatrix(mp);
            m = mp * m;
        }
    }
}

void Transform::GetNormalMatrix(Matrix4 &m) const
{
    GetModelMatrix(m);
    m = m.Inversed().Transposed();
}


void Transform::LookAt(Vector3 target, Vector3 _up)
{
    Assert(target != m_localPosition, "LookAt target is the same as position.", return);
    Vector3 up = _up.Normalized();
    SetRotation(Quaternion::LookDirection(target - GetPosition(), up) );
}

Vector3 Transform::GetLocalPosition() const
{
    return m_localPosition;
}

Vector3 Transform::GetPosition() const
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) return pt->GetPosition() + GetLocalPosition();
    }
    return GetLocalPosition();
}

Quaternion Transform::GetLocalRotation() const
{
    return m_localRotation;
}

Quaternion Transform::GetRotation() const
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) return pt->GetRotation() * GetLocalRotation();
    }
    return GetLocalRotation();
}

Vector3 Transform::GetLocalEuler() const
{
    return m_localEuler;
}

Vector3 Transform::GetEuler() const
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) return pt->GetEuler() + GetLocalEuler();
    }
    return GetLocalEuler();
}

Vector3 Transform::GetLocalScale() const
{
    return m_localScale;
}

Vector3 Transform::GetScale() const
{
    if (gameObject->parent)
    {
        Transform *pt = gameObject->parent->transform;
        if (pt) return pt->GetScale() * GetLocalScale();
    }
    return GetLocalScale();
}

Vector3 Transform::GetForward() const
{
    return  LocalToWorldDirection(Vector3::forward).Normalized();
}

Vector3 Transform::GetBack() const
{
    return -GetForward();
}

Vector3 Transform::GetRight() const
{
    return  LocalToWorldDirection(Vector3::right).Normalized();
}

Vector3 Transform::GetLeft() const
{
    return -GetRight();
}

Vector3 Transform::GetUp() const
{
    return LocalToWorldDirection(Vector3::up).Normalized();
}

Vector3 Transform::GetDown() const
{
    return -GetUp();
}


const std::string Transform::ToString() const
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

InspectorWidgetInfo* Transform::GetComponentInfo()
{
    Vector3 pos = GetPosition();
    Vector3 rotEuler = GetEuler();
    Vector3 scale = GetScale();

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(0))->m_value =
        {pos.x, pos.y, pos.z};

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(1))->m_value =
        {rotEuler.x, rotEuler.y, rotEuler.z};

    static_cast<InspectorVFloatSWInfo*>(m_inspectorComponentInfo.GetSlotInfo(2))->m_value =
        {scale.x, scale.y, scale.z};

    return &m_inspectorComponentInfo;
}

void Transform::OnSlotValueChanged(InspectorWidget *source)
{
    std::vector<float> v;
    v = source->GetSWVectorFloatValue("Position");
    SetPosition(Vector3(v[0], v[1], v[2]));

    v = source->GetSWVectorFloatValue("Rotation");
    SetEuler(Vector3(v[0], v[1], v[2]));

    v = source->GetSWVectorFloatValue("Scale");
    SetScale(Vector3(v[0], v[1], v[2]));
}
#endif

void Transform::Write(std::ostream &f) const
{
    f << "<Transform>" << std::endl;
    f << ((void*)this) << std::endl;
    FileWriter::Write(GetLocalPosition(), f);
    FileWriter::Write(GetLocalRotation(), f);
    FileWriter::Write(GetLocalScale(), f);
    f << "</Transform>" << std::endl;
}

void Transform::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this);
    SetLocalPosition(FileReader::ReadVec3(f));
    SetLocalRotation(FileReader::ReadQuat(f));
    SetLocalScale(FileReader::ReadVec3(f));
    FileReader::ReadNextLine(f); //Consume close tag
}
