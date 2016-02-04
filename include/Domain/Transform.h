#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Transform
{
public:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform();
};

#endif // TRANSFORM_H
