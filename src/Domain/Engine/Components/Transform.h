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

    Vector3 m_localPosition;
    Quaternion m_localRotation;
    Vector3 m_localScale;

    // Used in inspector too
    Vector3 m_localEuler;

public:

    Transform();
    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;
    virtual ~Transform();

    void LookAt(const Vector3 &target,
                const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));
    void LookInDirection(const Vector3 &dir,
                const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));


    void SetLocalPosition(const Vector3 &p);
    void SetPosition(const Vector3 &p);
    void TranslateLocal(const Vector3 &translation);
    void Translate(const Vector3 &translation);

    void SetLocalRotation(const Quaternion &r);
    void SetLocalEuler(const Vector3 &degreesEuler);
    void SetLocalEuler(float x, float y, float z);

    void SetRotation(const Quaternion &r);
    void SetEuler(const Vector3 &degreesEuler);
    void SetEuler(float x, float y, float z);

    void RotateLocal(const Quaternion &r);
    void RotateLocalEuler(const Vector3 &degreesEuler);
    void Rotate(const Quaternion &r);
    void RotateEuler(const Vector3 &degreesEuler);

#ifdef BANG_EDITOR
    void SetLocalRotationFromInspector(const Quaternion &q);
#endif

    void SetScale(float s);
    void SetScale(const Vector3 &v);
    void SetLocalScale(float s);
    void SetLocalScale(const Vector3 &s);

    /**
     * @brief Transforms the passed direction from local(parent) space to world space.
     * @return
     */
    Vector3 TransformPoint(const Vector3 &point) const;
    Vector3 InverseTransformPoint(const Vector3 &point) const;
    Vector3 TransformDirection(const Vector3 &dir) const;
    Vector3 InverseTransformDirection(const Vector3 &dir) const;

    // These are aliases of the above
    Vector3 LocalToWorldPoint(const Vector3 &point) const;
    Vector3 WorldToLocalPoint(const Vector3 &point) const;
    Vector3 LocalToWorldDirection(const Vector3 &dir) const;
    Vector3 WorldToLocalDirection(const Vector3 &dir) const;

    Vector3 LocalToObjectPoint(const Vector3 &point) const;
    Vector3 LocalToObjectDirection(const Vector3 &dir) const;
    Vector3 ObjectToLocalPoint(const Vector3 &point) const;
    Vector3 ObjectToLocalDirection(const Vector3 &dir) const;

    Vector3 ObjectToWorldPoint(const Vector3 &point) const;
    Vector3 ObjectToWorldDirection(const Vector3 &dir) const;
    Vector3 WorldToObjectPoint(const Vector3 &point) const;
    Vector3 WorldToObjectDirection(const Vector3 &dir) const;


    void GetObjectModelMatrix(Matrix4 *m) const;
    void GetModelMatrix(Matrix4 *m) const;

    void GetNormalMatrix(Matrix4 *m) const;

    Vector3 GetForward() const;
    Vector3 GetBack() const;
    Vector3 GetRight() const;
    Vector3 GetLeft() const;
    Vector3 GetUp() const;
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
    virtual InspectorWidgetInfo* OnInspectorInfoNeeded() override;
    virtual void OnInspectorInfoChanged(InspectorWidgetInfo *info) override;
    #endif

    void WriteInternal(std::ostream &f) const override;
    void ReadInternal(std::istream &f) override;
};

#endif // TRANSFORM_H
