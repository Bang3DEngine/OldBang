#include "Transform.h"

Transform::Transform() : position(glm::vec3(0.0f)),
                         rotation(glm::quat()),
                         scale(glm::vec3(1.0f))
{
}

void Transform::GetMatrix(glm::mat4 &m) const
{
    m = glm::mat4(1.0f);

    glm::mat4 T = glm::translate(m, position);
    glm::mat4 R = glm::mat4_cast(rotation);
    glm::mat4 S = glm::scale(m, scale);

    m = T * R * S;
}
