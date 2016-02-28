#include "Transform.h"
#include "StageReader.h"
#include "Entity.h"

Transform::Transform() : position(glm::vec3(0.0f)),
                         rotation(glm::quat()),
                         scale(glm::vec3(1.0f))
{
    #ifdef BANG_EDITOR
    glm::vec3 euler = glm::eulerAngles(rotation);
    inspectorPartInfo.slotInfos =
    {
        new InspectorPartInfoSlotVecFloat(
            "Position", {position.x, position.y, position.z}
        ),
        new InspectorPartInfoSlotVecFloat(
            "Rotation", {glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z)}
        ),
        new InspectorPartInfoSlotVecFloat(
            "Scale", {scale.x, scale.y, scale.z}
        )
    };
    #endif
}

Transform::~Transform()
{
}

void Transform::SetPosition(const glm::vec3 &p)
{
    position = p;
}

void Transform::SetRotation(const glm::vec3 &euler)
{
    glm::quat qx = glm::angleAxis(glm::radians(euler.x), glm::vec3(1,0,0));
    glm::quat qy = glm::angleAxis(glm::radians(euler.y), glm::vec3(0,1,0));
    glm::quat qz = glm::angleAxis(glm::radians(euler.z), glm::vec3(0,0,1));
    rotation = qx * qy * qz;
}

void Transform::SetRotation(const glm::quat &q)
{
    rotation = glm::normalize(q);
}

void Transform::SetScale(const glm::vec3 &s)
{
    scale = s;
}

void Transform::GetMatrix(glm::mat4 &m) const
{
    glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 R = glm::mat4_cast(rotation);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);

    m = T * R * S;

    Entity *parent = GetOwner()->GetParent();
    if(parent != nullptr)
    {
        Transform *tp = parent->GetPart<Transform>();
        if(tp != nullptr)
        {
            glm::mat4 mp = glm::mat4(1.0f);
            tp->GetMatrix(mp);
            m = mp * m;
        }
    }
}

void Transform::GetNormalMatrix(glm::mat4 &m) const
{
    GetMatrix(m);
    m = glm::transpose(glm::inverse(m));
}

void Transform::LookAt(glm::vec3 target)
{
    Assert(target != position, "LookAt target is the same as position.", return);

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
}

glm::vec3 Transform::GetPosition() const
{
    return position;
}

glm::quat Transform::GetRotation() const
{
    return rotation;
}

glm::vec3 Transform::GetScale() const
{
    return scale;
}
glm::vec3 Transform::GetForward() const
{
    return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 Transform::GetBack() const
{
    return -GetForward();
}

glm::vec3 Transform::GetRight() const
{
    return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::GetLeft() const
{
    return -GetRight();
}

glm::vec3 Transform::GetUp() const
{
    return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
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
           "   Rotation: (" << euler.x << ", " << euler.y << ", " << euler.z << ")" << std::endl <<
           "   Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl <<
           "]" << std::endl;

    return msg.str();
}

void Transform::Write(std::ostream &f) const
{

}

void Transform::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this);
    SetPosition(FileReader::ReadVec3(f));
    SetRotation(FileReader::ReadQuat(f));
    SetScale(FileReader::ReadVec3(f));
    FileReader::ReadNextLine(f); //Consume close tag
}

#ifdef BANG_EDITOR
InspectorPartInfo* Transform::GetInfo()
{
    static_cast<InspectorPartInfoSlotVecFloat*>(inspectorPartInfo.slotInfos[0])->value =
        {position.x, position.y, position.z};
    glm::vec3 euler = glm::eulerAngles(rotation);
    static_cast<InspectorPartInfoSlotVecFloat*>(inspectorPartInfo.slotInfos[1])->value =
        {glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z)};
    static_cast<InspectorPartInfoSlotVecFloat*>(inspectorPartInfo.slotInfos[2])->value =
        {scale.x, scale.y, scale.z};

    return &inspectorPartInfo;
}

void Transform::OnInspectorSlotChanged(InspectorPartWidget *partWidget)
{
    Logger_Log("_______________________________________");
    Logger_Log(this);

    std::vector<float> v;
    v = partWidget->GetVectorFloatSlotValue("Position");
    Logger_Log("pos v: " << v);
    position = glm::vec3(v[0], v[1], v[2]);

    v = partWidget->GetVectorFloatSlotValue("Rotation");
    Logger_Log("rot v: " << v);
    SetRotation(glm::vec3(v[0], v[1], v[2]));

    v = partWidget->GetVectorFloatSlotValue("Scale");
    Logger_Log("scale v: " << v);
    scale = glm::vec3(v[0], v[1], v[2]);
}
#endif
