#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Part.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class Transform : public Part
{
public:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    Transform();
    void GetMatrix(glm::mat4 &m) const;
};

#endif // TRANSFORM_H
