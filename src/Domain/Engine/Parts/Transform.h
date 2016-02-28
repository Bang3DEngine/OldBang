#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Bang.h"

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
    virtual ~Transform();

    void GetMatrix(glm::mat4 &m) const;
    void GetNormalMatrix(glm::mat4 &m) const;

    void LookAt(glm::vec3 target);

    void SetPosition(const glm::vec3 &p);
    void SetRotation(const glm::vec3 &euler);
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

    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;

    virtual std::string GetName() const override { return "Transform"; }

    #ifdef BANG_EDITOR
    virtual ListInspectorPartInfo* GetInfo() override;
    virtual void OnInspectorSlotChanged(ListInspectorPartWidget *partWidget) override;
    #endif
};

#endif // TRANSFORM_H
