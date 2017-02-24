#ifndef CAMERA_H
#define CAMERA_H

#include "Rect.h"
#include "AABox.h"
#include "Color.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "NamedEnum.h"
#include "Component.h"
#include "IRenderAgent.h"

class Mesh;
class Screen;
class Matrix4;
class XMLNode;
class ICloneable;
class MeshRenderer;
class ShaderProgram;
class Camera : public Component,
               public IRenderAgent
{
public:
    NamedEnum (ProjectionMode,
        Orthographic,
        Perspective
    );

    void SetReplacementShaderProgram(ShaderProgram *replacementShaderProgram);
    ShaderProgram *GetReplacementShaderProgram() const;

    virtual void OnRenderingStarts(GameObject *go,
                                   ShaderProgram *sp) override;

    void GetViewMatrix(Matrix4 *view) const;
    void GetProjectionMatrix(Matrix4 *proj) const;

    void SetOrthoHeight(float orthoHeight);

    void SetClearColor(const Color& color);
    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);

    void SetProjectionMode(ProjectionMode projMode);

    const Color& GetClearColor() const;
    float GetOrthoWidth() const;
    float GetOrthoHeight() const;
    float GetFovDegrees() const;
    float GetZNear() const;
    float GetZFar() const;
    ProjectionMode GetProjectionMode() const;

    /**
     * @brief Converts a point from world coods to Screen NDC.
     *        NDC: Normalized Device Coordinates [(-1,-1) -> (1,1)]
    **/
    Vector2 WorldToScreenNDCPoint(const Vector3 &position);
    Vector3 ScreenNDCPointToWorld(const Vector2 &screenNDCPos, float zFromCamera);

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;


    Rect GetScreenBoundingRect(const AABox &bbox);

    #ifdef BANG_EDITOR
    void OnDrawGizmos() override;
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void SetIdentityMode(bool identityMode);
    bool IsInIdentityMode() const;

protected:
    Camera();
    virtual ~Camera();

private:
    ShaderProgram *m_replacementShaderProgram = nullptr;
    float m_orthoHeight  = 25.0f;
    bool m_identityMode = false;
    Mesh *p_camMesh = nullptr;

    Color m_clearColor = Color(Color(0.4f), 1);
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;

    ProjectionMode m_projMode = ProjectionMode::Perspective;

    friend class GameObject;
};

#endif // CAMERA_H
