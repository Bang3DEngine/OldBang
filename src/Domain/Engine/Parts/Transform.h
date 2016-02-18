#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <string>
#include <sstream>
#include <ostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "FileReader.h"
#include "Part.h"

class Transform : public Part
{
private:

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

public:

    Transform();
    void GetMatrix(glm::mat4 &m) const;
    void GetNormalMatrix(glm::mat4 &m) const;

    void LookAt(glm::vec3 target);

    void SetPosition(const glm::vec3 &p);
    void SetRotation(const glm::quat &r);
    void SetScale(const glm::vec3 &s);

    glm::vec3 GetForward() const;
    glm::vec3 GetBack() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetLeft() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetDown() const;

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;

    const std::string ToString() const override;

    void Write(std::ofstream &f) const override;
    void Read(std::ifstream &f) override;
};

#endif // TRANSFORM_H
