#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Bang/Cached.h"
#include "Bang/Vector3.h"
#include "Bang/Matrix4.h"
#include "Bang/Component.h"
#include "Bang/Quaternion.h"
#include "Bang/IInvalidatable.h"
#include "Bang/IChildrenListener.h"
#include "Bang/ITransformListener.h"

NAMESPACE_BANG_BEGIN

class Transform : public Component,
                  public IInvalidatable<Transform>,
                  public ITransformListener,
                  public IChildrenListener,
                  public EventEmitter<ITransformListener>
{
    COMPONENT(Transform)

public:
    void LookAt(const Vector3 &target, const Vector3 &up = Vector3::Up);
    void LookAt(Transform *targetTransform, const Vector3 &up = Vector3::Up);
    void LookAt(GameObject *target, const Vector3 &up = Vector3::Up);
    void LookInDirection(const Vector3 &dir, const Vector3 &up = Vector3::Up);

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

    Vector3 FromLocalToWorldPoint(const Vector3 &point) const;
    Vector3 FromLocalToWorldVector(const Vector3 &vector) const;
    Vector3 FromLocalToWorldDirection(const Vector3 &dir) const;
    Vector3 FromWorldToLocalPoint(const Vector3 &point) const;
    Vector3 FromWorldToLocalVector(const Vector3 &vector) const;
    Vector3 FromWorldToLocalDirection(const Vector3 &dir) const;

    const Matrix4& GetLocalToParentMatrix() const;
    const Matrix4& GetLocalToParentMatrixInv() const;
    const Matrix4& GetLocalToWorldMatrix() const;
    const Matrix4& GetLocalToWorldMatrixInv() const;

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

    // IChildrenListener
    void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

    // ITransformListener
    void OnTransformChanged() override;
    void OnParentTransformChanged() override;
    void OnChildrenTransformChanged() override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    // IInvalidatable
    void OnInvalidated() override;

protected:
    mutable Matrix4 m_localToWorldMatrix;
    mutable Matrix4 m_localToWorldMatrixInv;
    mutable Matrix4 m_localToParentMatrix;
    mutable Matrix4 m_localToParentMatrixInv;
    mutable bool m_invalidLocalToWorldMatrix = true;

    Transform();
    virtual ~Transform();

    virtual bool CanBeRepeatedInGameObject() const override;

private:
    Vector3    m_localPosition  = Vector3::Zero;
    Quaternion m_localRotation = Quaternion::Identity;
    Vector3    m_localScale    = Vector3::One;

    void RecalculateParentMatricesIfNeeded() const;
    void RecalculateWorldMatricesIfNeeded() const;
    virtual void CalculateLocalToParentMatrix() const;
    virtual void CalculateLocalToWorldMatrix() const;

    void PropagateParentTransformChangedEventToChildren() const;
    void PropagateChildrenTransformChangedEventToParent() const;
};

NAMESPACE_BANG_END

#endif // TRANSFORM_H
