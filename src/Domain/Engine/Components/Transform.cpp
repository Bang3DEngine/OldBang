#include "Transform.h"
#include "FileReader.h"
#include "GameObject.h"

Transform::Transform() : localPosition(Vector3(0.0f)),
                         localRotation(Quaternion()),
                         localScale(Vector3(1.0f)),
                         inspectorEulerDeg(Vector3(0.0f))
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo(
            "Position", {localPosition.x, localPosition.y, localPosition.z}
        ),
        new InspectorVFloatSWInfo(
            "Rotation", {inspectorEulerDeg.x,
                         inspectorEulerDeg.y,
                         inspectorEulerDeg.z}
        ),
        new InspectorVFloatSWInfo(
            "Scale", {localScale.x, localScale.y, localScale.z}
        )
    });
#endif
}

void Transform::CloneInto(ICloneable *clone) const
{
    Component::CloneInto(clone);
    Transform *t = static_cast<Transform*>(clone);
    t->SetPosition(GetLocalPosition());
    t->SetRotation(GetLocalRotation());
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
    localPosition = p;
}
void Transform::SetPosition(const Vector3 &p)
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if (pt) SetLocalPosition(-pt->GetPosition() + p);
        else SetLocalPosition(p);
    }
    else SetLocalPosition(p);
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
    localRotation = q.Normalized();
    inspectorEulerDeg = Quaternion::EulerAngles(localRotation).ToDegrees();
}
void Transform::SetLocalEuler(const Vector3 &degreesEuler)
{
    inspectorEulerDeg = degreesEuler;
    Vector3 rads = inspectorEulerDeg.ToRadians();
    Quaternion qx = Quaternion::AngleAxis(rads.x, Vector3::right);
    Quaternion qy = Quaternion::AngleAxis(rads.y, Vector3::up);
    Quaternion qz = Quaternion::AngleAxis(rads.z, Vector3::forward);
    localRotation = (qz * qy * qx).Normalized();
}
void Transform::SetLocalEuler(float x, float y, float z)
{
    SetLocalEuler(Vector3(x,y,z));
}

void Transform::SetRotation(const Quaternion &q)
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if (pt) SetLocalRotation(Quaternion(-pt->GetRotation() * q.Normalized()));
        else SetLocalRotation(q);
    }
    else SetLocalRotation(q);
}
void Transform::SetEuler(const Vector3 &degreesEuler)
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
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

void Transform::SetLocalRotationFromInspector(const Quaternion &q)
{
    localRotation = q.Normalized();
}




void Transform::SetScale(float s)
{
    SetScale(Vector3(s));
}

void Transform::SetScale(const Vector3 &v)
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
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
    localScale = s;
}

void Transform::SetLeftMatrix(const Matrix4 &leftMatrix)
{
    this->leftMatrix = leftMatrix;
}

void Transform::SetRightMatrix(const Matrix4 &rightMatrix)
{
    this->rightMatrix = rightMatrix;
}

void Transform::GetLocalModelMatrix(Matrix4 &m) const
{
    Matrix4 T = Matrix4::TranslateMatrix(GetLocalPosition());
    Matrix4 R = Matrix4::RotateMatrix(GetLocalRotation());
    Matrix4 S = Matrix4::ScaleMatrix(GetLocalScale());

    m = leftMatrix * T * R * S * rightMatrix;
}

void Transform::GetModelMatrix(Matrix4 &m) const
{
    GetLocalModelMatrix(m);

    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *tp = parent->GetComponent<Transform>();
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
    Assert(target != localPosition, "LookAt target is the same as position.", return);
    Vector3 up = _up.Normalized();
    SetRotation(Quaternion::LookDirection(target - GetPosition(), up) );
}

Vector3 Transform::GetLocalPosition() const
{
    return localPosition;
}

Vector3 Transform::GetPosition() const
{
    GameObject *parent = owner->GetParent();
    if(parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if(pt) return pt->GetPosition() + GetLocalPosition();
    }
    return GetLocalPosition();
}

Quaternion Transform::GetLocalRotation() const
{
    return localRotation;
}

Quaternion Transform::GetRotation() const
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if (pt) return pt->GetRotation() * GetLocalRotation();
    }
    return GetLocalRotation();
}

Vector3 Transform::GetLocalEuler() const
{
    return inspectorEulerDeg;
}

Vector3 Transform::GetEuler() const
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if (pt) return pt->GetEuler() + GetLocalEuler();
    }
    return GetLocalEuler();
}

Vector3 Transform::GetLocalScale() const
{
    return localScale;
}

Vector3 Transform::GetScale() const
{
    GameObject *parent = owner->GetParent();
    if (parent)
    {
        Transform *pt = parent->GetComponent<Transform>();
        if (pt) return pt->GetScale() * GetLocalScale();
    }
    return GetLocalScale();
}

Vector3 Transform::GetLocalForward() const
{
    return (GetLocalRotation() * Vector3::forward).Normalized();
}

Vector3 Transform::GetForward() const
{
    return  (GetRotation() * Vector3::forward).Normalized();
}

Vector3 Transform::GetLocalBack() const
{
    return -GetLocalForward();
}

Vector3 Transform::GetBack() const
{
    return -GetForward();
}

Vector3 Transform::GetLocalRight() const
{
    return (GetLocalRotation() * Vector3::right).Normalized();
}

Vector3 Transform::GetRight() const
{
    return  (GetRotation() * Vector3::right).Normalized();
}

Vector3 Transform::GetLocalLeft() const
{
    return -GetLocalRight();
}

Vector3 Transform::GetLeft() const
{
    return -GetRight();
}

Vector3 Transform::GetLocalUp() const
{
    return (GetLocalRotation() * Vector3::up).Normalized();
}

Vector3 Transform::GetUp() const
{
    return (GetRotation() * Vector3::up).Normalized();
}

Vector3 Transform::GetLocalDown() const
{
    return -GetLocalUp();
}

Vector3 Transform::GetDown() const
{
    return -GetUp();
}


const std::string Transform::ToString() const
{
    using std::operator<<;

    std::ostringstream msg;

    Vector3 euler = Quaternion::EulerAngles(localRotation);
    msg << "[" <<
           "Transform: " << std::endl <<
           "   Position: (" << localPosition.x << ", " << localPosition.y << ", " << localPosition.z << ")"  << std::endl <<
           "   Rotation: (" << glm::degrees(euler.x) << ", " << glm::degrees(euler.y) << ", " << glm::degrees(euler.z) << ")" << std::endl <<
           "   Scale: (" << localScale.x << ", " << localScale.y << ", " << localScale.z << ")" << std::endl <<
           "]" << std::endl;

    return msg.str();
}
#ifdef BANG_EDITOR

InspectorWidgetInfo* Transform::GetComponentInfo()
{
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(0))->value =
        {localPosition.x, localPosition.y, localPosition.z};

    Vector3 e = inspectorEulerDeg;
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(1))->value =
        {e.x, e.y, e.z};

    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(2))->value =
        {localScale.x, localScale.y, localScale.z};

    return &inspectorComponentInfo;
}

void Transform::OnSlotValueChanged(InspectorWidget *source)
{
    std::vector<float> v;
    v = source->GetSWVectorFloatValue("Position");
    localPosition = Vector3(v[0], v[1], v[2]);

    v = source->GetSWVectorFloatValue("Rotation");
    SetEuler(Vector3(v[0], v[1], v[2]));

    v = source->GetSWVectorFloatValue("Scale");
    localScale = Vector3(v[0], v[1], v[2]);
}

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

#endif
