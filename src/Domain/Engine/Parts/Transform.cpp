#include "Transform.h"

Transform::Transform() : position(glm::vec3(0.0f)),
                         rotation(glm::quat()),
                         scale(glm::vec3(1.0f))
{
}

void Transform::SetPosition(const glm::vec3 &p)
{
    position = p;
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

    msg << "[" <<
           "Transform: " << std::endl <<
           "   Position: (" << position.x << ", " << position.y << ", " << position.z << ")"  << std::endl <<
           "   Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << "," << rotation.w << ")" << std::endl <<
           "   Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl <<
           "]" << std::endl;

    return msg.str();
}
