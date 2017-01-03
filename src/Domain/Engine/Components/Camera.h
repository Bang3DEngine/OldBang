#ifndef CAMERA_H
#define CAMERA_H

#include "Color.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "NamedEnum.h"
#include "Component.h"


class Mesh;
class Screen;
class Matrix4;
class XMLNode;
class ICloneable;
class MeshRenderer;
class Camera : public Component
{
public:
    NamedEnum (ProjectionMode,
        Orthographic,
        Perspective
    );

    void GetViewMatrix(Matrix4 *view) const;
    void GetProjectionMatrix(Matrix4 *proj) const;

    void SetOrthoHeight(float orthoHeight);

    void SetClearColor(const Color& color);
    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);

    void SetProjectionMode(ProjectionMode projMode);

    void SetAspectRatio(float aspectRatio);

    void SetAutoUpdateAspectRatio(bool autoUpdateAspectRatio);

    const Color& GetClearColor() const;
    float GetOrthoWidth() const;
    float GetOrthoHeight() const;
    float GetFovDegrees() const;
    float GetAspectRatio() const;
    float GetZNear() const;
    float GetZFar() const;
    ProjectionMode GetProjectionMode() const;
    bool  GetAutoUpdateAspectRatio() const;

    /**
     * @brief Converts a point from world coods to Screen NDC.
     *        NDC: Normalized Device Coordinates [(-1,-1) -> (1,1)]
    **/
    Vector2 WorldToScreenNDCPoint(const Vector3 &position);
    Vector3 ScreenNDCPointToWorld(const Vector2 &screenNDCPos, float zFromCamera);

    virtual const String ToString() const override;
    virtual String GetName() const override { return "Camera"; }

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    void OnDrawGizmos() override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Camera();

private:
    static bool s_inited;
    static Mesh* s_camMesh;
    static void InitStatics();

    float m_orthoHeight  = 0.5f;

    Color m_clearColor = Color(Color(0.4f), 1);
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;

    mutable float m_aspectRatio = 1.0f;

    ProjectionMode m_projMode = ProjectionMode::Perspective;

    bool m_autoUpdateAspectRatio = true;

    friend class GameObject;
};

#endif // CAMERA_H
