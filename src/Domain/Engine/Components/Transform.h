#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "String.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Property.h"
#include "Component.h"
#include "Quaternion.h"

class XMLNode;
class ICloneable;
class Transform : public Component
{
    OBJECT(Transform)
    ICLONEABLE(Transform)

public:
    virtual void CloneInto(ICloneable *clone) const override;

    virtual ~Transform();

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

    /**
     * @brief Transforms the point from local space to world space.
     * @return
     */
    Vector3 TransformPoint(const Vector3 &point) const;

    /**
     * @brief Transforms the point from world space to local space.
     * @return
     */
    Vector3 InverseTransformPoint(const Vector3 &point) const;

    /**
     * @brief Transforms the direction from local space to world space.
     * @return
     */
    Vector3 TransformDirection(const Vector3 &dir) const;

    /**
     * @brief Transforms the direction from world space to local space.
     * @return
     */
    Vector3 InverseTransformDirection(const Vector3 &dir) const;

    Vector3 ParentToLocalPoint(const Vector3 &point) const;
    Vector3 ParentToLocalDirection(const Vector3 &dir) const;
    Vector3 LocalToParentPoint(const Vector3 &point) const;
    Vector3 LocalToParentDirection(const Vector3 &dir) const;

    Vector3 LocalToWorldPoint(const Vector3 &point) const;
    Vector3 LocalToWorldDirection(const Vector3 &dir) const;
    Vector3 WorldToLocalPoint(const Vector3 &point) const;
    Vector3 WorldToLocalDirection(const Vector3 &dir) const;


    virtual const Matrix4& GetLocalToParentMatrix() const;
    void GetLocalToParentMatrix(Matrix4 *m) const;
    void GetLocalToWorldMatrix(Matrix4 *m) const;

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

    static Vector3    GetPositionFromMatrix4(const Matrix4 &transformMatrix);
    static Quaternion GetRotationFromMatrix4(const Matrix4 &transformMatrix);
    static Vector3    GetScaleFromMatrix4   (const Matrix4 &transformMatrix);
    static Transform  FromTransformMatrix(const Matrix4 &transformMatrix);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Transform();

    mutable bool m_hasChanged = false;
    mutable Matrix4 m_localToParentMatrix;
    Vector3 m_localPosition = Vector3::Zero;
    Quaternion m_localRotation = Quaternion::Identity;
    Vector3 m_localScale = Vector3::One;

    // Used in inspector too
    Vector3 m_localEuler = Vector3::Zero;
};

#endif // TRANSFORM_H
