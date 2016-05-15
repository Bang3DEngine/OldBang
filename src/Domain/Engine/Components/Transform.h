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
#include "Component.h"

class Transform : public Component
{

private:

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    // euler angles in degrees set by user in the inspector
    glm::vec3 inspectorEulerDeg;

    // L * MODEL * R, user can edit those in order to
    // apply its own post/pre transformations
    glm::mat4 leftMatrix;
    glm::mat4 rightMatrix;

public:

    Transform();
    virtual ~Transform();

    void LookAt(glm::vec3 target,
                glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));



    void SetPosition(const glm::vec3 &p);

    //These wont change the input inspectorEulerDeg of the
    void Rotate(const glm::vec3 &degreesEuler);
    void SetRotation(const glm::vec3 &degreesEuler);
    void SetRotationFromInspector(const glm::quat &q);

    //This will change inspectorEulerDeg to the quat->eulerAngles conversion
    void SetRotation(const glm::quat &r);
    void Rotate(const glm::quat &r);

    void SetScale(float s);
    void SetScale(const glm::vec3 &s);
    void SetLeftMatrix(const glm::mat4 &leftMatrix);
    void SetRightMatrix(const glm::mat4 &rightMatrix);

    void GetLocalMatrix(glm::mat4 &m) const;
    void GetMatrix(glm::mat4 &m) const;

    void GetNormalMatrix(glm::mat4 &m) const;

    glm::vec3 GetLocalForward() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetLocalBack() const;
    glm::vec3 GetBack() const;
    glm::vec3 GetLocalRight() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetLocalLeft() const;
    glm::vec3 GetLeft() const;
    glm::vec3 GetLocalUp() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetLocalDown() const;
    glm::vec3 GetDown() const;

    glm::vec3 GetLocalPosition() const;
    glm::vec3 GetPosition() const;
    glm::quat GetLocalRotation() const;
    glm::quat GetRotation() const;
    glm::vec3 GetLocalEuler() const;
    glm::vec3 GetEuler() const; // Gets rotation in euler angles
    glm::vec3 GetLocalScale() const;
    glm::vec3 GetScale() const;

    const std::string ToString() const override;


    virtual std::string GetName() const override { return "Transform"; }

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;
#endif
};

#endif // TRANSFORM_H
