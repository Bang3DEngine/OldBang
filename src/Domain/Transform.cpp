#include "Transform.h"

Transform::Transform()
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
