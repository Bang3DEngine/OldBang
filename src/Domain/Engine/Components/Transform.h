#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Bang.h"


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
public:

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


    const Matrix4& GetLocalToParentMatrix() const;
    void GetLocalToParentMatrix(Matrix4 *m) const;
    void GetLocalToWorldMatrix(Matrix4 *m) const;

    void GetLocalToWorldNormalMatrix(Matrix4 *m) const;

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

    const String ToString() const override;
    virtual String GetName() const override { return "Transform"; }

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Transform();

private:

    mutable bool m_hasChanged = false;
    mutable Matrix4 m_localToWorldMatrix;
    Vector3 m_localPosition = Vector3(0.0f);
    Quaternion m_localRotation = Quaternion();
    Vector3 m_localScale = Vector3(1.0f);

    // Used in inspector too
    Vector3 m_localEuler = Vector3(0.0f);

    friend class GameObject;
};

#endif // TRANSFORM_H
