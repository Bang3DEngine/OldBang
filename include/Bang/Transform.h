#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Bang/Cached.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"
#include "Bang/Component.h"
#include "Bang/Quaternion.h"

NAMESPACE_BANG_BEGIN

class Transform : public Component
{
    COMPONENT(Transform)

public:
    virtual ~Transform();

    virtual const Matrix4& GetLocalToParentMatrix() const;

    void LookAt(const Vector3 &target,
                const Vector3 &up = Vector3::Up);
    void LookAt(Transform *targetTransform,
                const Vector3 &up = Vector3::Up);
    void LookAt(GameObject *target,
                const Vector3 &up = Vector3::Up);
    void LookInDirection(const Vector3 &dir,
                         const Vector3 &up = Vector3::Up);

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

    void SetScale(float s);
    void SetScale(const Vector3 &v);
    void SetLocalScale(float s);
    void SetLocalScale(const Vector3 &s);

    Vector3 TransformPoint(const Vector3 &point) const;
    Vector3 InverseTransformPoint(const Vector3 &point) const;
    Vector3 TransformDirection(const Vector3 &dir) const;
    Vector3 InverseTransformDirection(const Vector3 &dir) const;
    Vector3 TransformVector(const Vector3 &vector) const;
    Vector3 InverseTransformVector(const Vector3 &vector) const;

    Vector3 ParentToLocalPoint(const Vector3 &point) const;
    Vector3 ParentToLocalVector(const Vector3 &vector) const;
    Vector3 ParentToLocalDirection(const Vector3 &dir) const;
    Vector3 LocalToParentPoint(const Vector3 &point) const;
    Vector3 LocalToParentVector(const Vector3 &vector) const;
    Vector3 LocalToParentDirection(const Vector3 &dir) const;

    Vector3 LocalToWorldPoint(const Vector3 &point) const;
    Vector3 LocalToWorldVector(const Vector3 &vector) const;
    Vector3 LocalToWorldDirection(const Vector3 &dir) const;
    Vector3 WorldToLocalPoint(const Vector3 &point) const;
    Vector3 WorldToLocalVector(const Vector3 &vector) const;
    Vector3 WorldToLocalDirection(const Vector3 &dir) const;

    void GetLocalToParentMatrix(Matrix4 *m) const;
    void GetLocalToWorldMatrix(Matrix4 *m) const;

    Vector3 GetForward() const;
    Vector3 GetBack() const;
    Vector3 GetRight() const;
    Vector3 GetLeft() const;
    Vector3 GetUp() const;
    Vector3 GetDown() const;

    const Vector3& GetLocalPosition() const;
    Vector3 GetPosition() const;
    const Quaternion& GetLocalRotation() const;
    Quaternion GetRotation() const;
    Vector3 GetLocalEuler() const;
    Vector3 GetEuler() const;
    const Vector3& GetLocalScale() const;
    Vector3 GetScale() const;

    static Vector3    GetPositionFromMatrix4(const Matrix4 &transformMatrix);
    static Quaternion GetRotationFromMatrix4(const Matrix4 &transformMatrix);
    static Vector3    GetScaleFromMatrix4   (const Matrix4 &transformMatrix);
    static Transform  FromTransformMatrix(const Matrix4 &transformMatrix);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    bool m_hasChanged = false;
    mutable Matrix4 m_localToParentMatrix;
    Vector3 m_localPosition = Vector3::Zero;
    Quaternion m_localRotation = Quaternion::Identity;
    Vector3 m_localScale = Vector3::One;

    Transform();
};

NAMESPACE_BANG_END

#endif // TRANSFORM_H
