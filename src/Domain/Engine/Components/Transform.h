#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Bang.h"

#include <string>
#include <sstream>
#include <ostream>

#include "Vector3.h"
#include "Matrix4.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Quaternion.h"

#include "FileReader.h"
#include "Component.h"

class Transform : public Component
{

private:

    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    // euler angles in degrees set by user in the inspector
    Vector3 inspectorEulerDeg;

    // L * MODEL * R, user can edit those in order to
    // apply its own post/pre transformations
    Matrix4 leftMatrix;
    Matrix4 rightMatrix;

public:

    Transform();
    virtual ~Transform();

    void LookAt(Vector3 target,
                Vector3 up = Vector3(0.0f, 1.0f, 0.0f));



    void SetPosition(const Vector3 &p);
    void Translate(const Vector3 &translation);

    //These wont change the input inspectorEulerDeg of the
    /**
     * @brief Rotate around global axis (axis in world space).
     * Same goes for all methods without "Local"
     */
    void Rotate(const Vector3 &degreesEuler);
    void SetRotation(const Vector3 &degreesEuler);
    void SetRotationFromInspector(const Quaternion &q);

    /**
     * @brief Rotate around local axis (axis in object space)
     * Same goes for all methods wiith "Local"
     */
    void RotateLocal(const Vector3 &degreesEuler);
    void SetRotationLocal(const Vector3 &degreesEuler);

    //This will change inspectorEulerDeg to the quat->eulerAngles conversion
    void SetRotation(float x, float y, float z);
    void SetRotationLocal(float x, float y, float z);
    void SetRotation(const Quaternion &r);
    void SetRotationLocal(const Quaternion &r);
    void Rotate(const Quaternion &r);
    void RotateLocal(const Quaternion &r);

    void SetLocalScale(float s);
    void SetLocalScale(const Vector3 &s);
    void SetLeftMatrix(const Matrix4 &leftMatrix);
    void SetRightMatrix(const Matrix4 &rightMatrix);

    void GetLocalModelMatrix(Matrix4 &m) const;
    void GetModelMatrix(Matrix4 &m) const;

    void GetNormalMatrix(Matrix4 &m) const;

    Vector3 GetLocalForward() const;
    Vector3 GetForward() const;
    Vector3 GetLocalBack() const;
    Vector3 GetBack() const;
    Vector3 GetLocalRight() const;
    Vector3 GetRight() const;
    Vector3 GetLocalLeft() const;
    Vector3 GetLeft() const;
    Vector3 GetLocalUp() const;
    Vector3 GetUp() const;
    Vector3 GetLocalDown() const;
    Vector3 GetDown() const;

    Vector3 GetLocalPosition() const;
    Vector3 GetPosition() const;
    Quaternion GetLocalRotation() const;
    Quaternion GetRotation() const;
    Vector3 GetLocalEuler() const;
    Vector3 GetEuler() const; // Gets rotation in euler angles
    Vector3 GetLocalScale() const;
    Vector3 GetScale() const;

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
