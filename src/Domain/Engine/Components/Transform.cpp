#include "Transform.h"
#include "FileReader.h"
#include "GameObject.h"

Transform::Transform() : position(glm::vec3(0.0f)),
                         rotation(glm::quat()),
                         scale(glm::vec3(1.0f)),
                         inspectorEulerDeg(glm::vec3(0.0f))
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorVFloatSWInfo(
            "Position", {position.x, position.y, position.z}
        ),
        new InspectorVFloatSWInfo(
            "Rotation", {inspectorEulerDeg.x,
                         inspectorEulerDeg.y,
                         inspectorEulerDeg.z}
        ),
        new InspectorVFloatSWInfo(
            "Scale", {scale.x, scale.y, scale.z}
        )
    });
    #endif
}

Transform::~Transform()
{
}

void Transform::SetPosition(const glm::vec3 &p)
{
    position = p;
}

void Transform::SetRotationFromInspector(const glm::quat &q)
{
    rotation = glm::normalize(q);
}

void Transform::SetRotation(const glm::vec3 &degreesEuler)
{
    inspectorEulerDeg = degreesEuler;
    glm::vec3 rads = glm::radians(inspectorEulerDeg);
    glm::quat qx = glm::angleAxis(rads.x, glm::vec3(1,0,0));
    glm::quat qy = glm::angleAxis(rads.y, glm::vec3(0,1,0));
    glm::quat qz = glm::angleAxis(rads.z, glm::vec3(0,0,1));
    SetRotationFromInspector(qz * qy * qx);
}

void Transform::SetRotation(const glm::quat &q)
{
    rotation = glm::normalize(q);
    inspectorEulerDeg = glm::degrees(glm::eulerAngles(rotation));
}

void Transform::Rotate(const glm::quat &r)
{
    SetRotation(r * GetLocalRotation());
    inspectorEulerDeg = glm::degrees(glm::eulerAngles(rotation));
}

void Transform::SetScale(float s)
{
    SetScale(glm::vec3(s));
}

void Transform::SetScale(const glm::vec3 &s)
{
    scale = s;
}

void Transform::SetLeftMatrix(const glm::mat4 &leftMatrix)
{
    this->leftMatrix = leftMatrix;
}

void Transform::SetRightMatrix(const glm::mat4 &rightMatrix)
{
    this->rightMatrix = rightMatrix;
}

void Transform::GetLocalMatrix(glm::mat4 &m,
                               IgnoreParentTransformMask mask) const
{
    glm::mat4 T(1.0f), R(1.0f), S(1.0f);

    if(mask & IgnorePosition > 0)
        T = glm::translate(glm::mat4(1.0f), GetLocalPosition());

    if(mask & IgnoreRotation > 0)
        R = glm::mat4_cast(GetLocalRotation());

    if(mask & IgnoreScale > 0)
        S = glm::scale(glm::mat4(1.0f), GetLocalScale());

    m = leftMatrix * T * R * S * rightMatrix;
}

void Transform::GetMatrix(glm::mat4 &m, bool _firstCall) const
{
    if(_firstCall) GetLocalMatrix(m);
    else //We are computing parent's transform, take into account ignore masks
        GetLocalMatrix(m, ignoreParentTransformMask);

    GameObject *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *tp = parent->GetComponent<Transform>();
        if(tp != nullptr)
        {
            glm::mat4 mp;
            tp->GetMatrix(mp, false); //Next recursion, firstCall=false
            m = mp * m;
        }
    }
}

void Transform::GetNormalMatrix(glm::mat4 &m) const
{
    GetMatrix(m);
    m = glm::transpose(glm::inverse(m));
}


void Transform::LookAt(glm::vec3 target, glm::vec3 _up)
{
    Assert(target != position, "LookAt target is the same as position.", return);

    glm::vec3 up = glm::normalize(_up);

    SetRotation( glm::quat_cast( glm::inverse( glm::lookAt(GetPosition(), target, up)) ) );
    /*
    glm::vec3 direction = glm::normalize(target - position);
    float dot = glm::dot(glm::vec3(0, 0, -1), direction);

    if (fabs(dot - (-1.0f)) < 0.000001f)
    {
        rotation = glm::angleAxis(3.141592f, glm::vec3(0, 1, 0));
        return;
    }
    else if (fabs(dot - (1.0f)) < 0.000001f)
    {
        rotation = glm::quat();
        return;
    }

    float angle = -glm::acos(dot);
    glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0, 0, -1), direction));
    rotation = glm::conjugate(glm::normalize(glm::angleAxis(angle, cross)));
    */
}

glm::vec3 Transform::GetLocalPosition() const
{
    return position;
}

glm::vec3 Transform::GetPosition() const
{
    GameObject *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *pt = GetOwner()->GetParent()->GetComponent<Transform>();
        if(pt != nullptr) return pt->GetPosition() + GetLocalPosition();
    }
    return GetLocalPosition();
}

glm::quat Transform::GetLocalRotation() const
{
    return rotation;
}

glm::quat Transform::GetRotation() const
{
    GameObject *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *pt = GetOwner()->GetParent()->GetComponent<Transform>();
        if(pt != nullptr) return pt->GetRotation() * GetLocalRotation();
    }
    return GetLocalRotation();
}

glm::vec3 Transform::GetLocalEuler() const
{
    return inspectorEulerDeg;
}

glm::vec3 Transform::GetEuler() const
{
    GameObject *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *pt = GetOwner()->GetParent()->GetComponent<Transform>();
        if(pt != nullptr) return pt->GetEuler() + GetLocalEuler();
    }
    return GetLocalEuler();
}

glm::vec3 Transform::GetLocalScale() const
{
    return scale;
}

glm::vec3 Transform::GetScale() const
{
    GameObject *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *pt = GetOwner()->GetParent()->GetComponent<Transform>();
        if(pt != nullptr) return pt->GetScale() * GetLocalScale();
    }
    return GetLocalScale();
}

glm::vec3 Transform::GetLocalForward() const
{
    return glm::normalize(GetLocalRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Transform::GetForward() const
{
    return GetRotation() * GetLocalForward();
}

glm::vec3 Transform::GetLocalBack() const
{
    return -GetLocalForward();
}

glm::vec3 Transform::GetBack() const
{
    return -GetForward();
}

glm::vec3 Transform::GetLocalRight() const
{
    return glm::normalize(GetLocalRotation() * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 Transform::GetRight() const
{
    return GetRotation() * GetLocalRight();
}

glm::vec3 Transform::GetLocalLeft() const
{
    return -GetLocalRight();
}

glm::vec3 Transform::GetLeft() const
{
    return -GetRight();
}

glm::vec3 Transform::GetLocalUp() const
{
    return glm::normalize(GetLocalRotation() * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Transform::GetUp() const
{
    return GetRotation() * GetLocalUp();
}

glm::vec3 Transform::GetLocalDown() const
{
    return -GetLocalUp();
}

glm::vec3 Transform::GetDown() const
{
    return -GetUp();
}


const std::string Transform::ToString() const
{
    using std::operator<<;

    std::ostringstream msg;

    glm::vec3 euler = glm::eulerAngles(rotation);
    msg << "[" <<
           "Transform: " << std::endl <<
           "   Position: (" << position.x << ", " << position.y << ", " << position.z << ")"  << std::endl <<
           "   Rotation: (" << glm::degrees(euler.x) << ", " << glm::degrees(euler.y) << ", " << glm::degrees(euler.z) << ")" << std::endl <<
           "   Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl <<
           "]" << std::endl;

    return msg.str();
}

#ifdef BANG_EDITOR

InspectorWidgetInfo* Transform::GetComponentInfo()
{
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(0))->value =
        {position.x, position.y, position.z};

    glm::vec3 e = inspectorEulerDeg;
    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(1))->value =
        {e.x, e.y, e.z};

    static_cast<InspectorVFloatSWInfo*>(inspectorComponentInfo.GetSlotInfo(2))->value =
        {scale.x, scale.y, scale.z};

    return &inspectorComponentInfo;
}

void Transform::OnSlotValueChanged(InspectorWidget *source)
{
    std::vector<float> v;
    v = source->GetSWVectorFloatValue("Position");
    position = glm::vec3(v[0], v[1], v[2]);

    v = source->GetSWVectorFloatValue("Rotation");
    SetRotation(glm::vec3(v[0], v[1], v[2]));

    v = source->GetSWVectorFloatValue("Scale");
    scale = glm::vec3(v[0], v[1], v[2]);
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
    SetPosition(FileReader::ReadVec3(f));
    SetRotation(FileReader::ReadQuat(f));
    SetScale(FileReader::ReadVec3(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#endif
