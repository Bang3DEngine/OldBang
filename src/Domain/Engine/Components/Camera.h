#ifndef CAMERA_H
#define CAMERA_H

#include "Bang/Rect.h"
#include "Bang/AABox.h"
#include "Bang/Color.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/Vector3.h"
#include "Bang/NamedEnum.h"
#include "Bang/Component.h"

class Mesh;
class Screen;
class Matrix4;
class XMLNode;
class ICloneable;
class MeshRenderer;
class ShaderProgram;
class Camera : public Component
{
    OBJECT(Camera)
    ICLONEABLE(Camera)
    COMPONENT_ICON(Camera, "Icons/CameraIcon.png")

public:
    NamedEnum (ProjectionMode,
        Orthographic,
        Perspective
    );

    void SetReplacementShaderProgram(ShaderProgram *replacementShaderProgram);
    ShaderProgram *GetReplacementShaderProgram() const;

    void GetViewMatrix(Matrix4 *view) const;
    void GetProjectionMatrix(Matrix4 *proj) const;

    void SetOrthoHeight(float orthoHeight);

    void SetClearColor(const Color& color);
    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);

    void SetProjectionMode(ProjectionMode projMode);

    virtual void Bind() const;

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

    virtual void CloneInto(ICloneable *clone) const override;

    Rect GetScreenBoundingRect(const AABox &bbox);

    #ifdef BANG_EDITOR
    void OnDrawGizmos(bool depthed, bool overlay) override;
    #endif

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

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

    Color m_clearColor = Color(Color(0.3f), 1);
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;

    ProjectionMode m_projMode = ProjectionMode::Perspective;
};

#endif // CAMERA_H
