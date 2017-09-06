#ifndef CAMERA_H
#define CAMERA_H

#include "Bang/Color.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class Screen;
FORWARD class ShaderProgram;

class Camera : public Component
{
    COMPONENT(Camera)

public:
    enum ProjectionMode
    {
        Orthographic,
        Perspective
    };


    virtual void Bind() const;
    Vector2 WorldToScreenNDCPoint(const Vector3 &position);
    Vector3 ScreenNDCPointToWorld(const Vector2 &screenNDCPos, float zFromCam);

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
    void GetViewMatrix(Matrix4 *view) const;
    void GetProjectionMatrix(Matrix4 *proj) const;
    ProjectionMode GetProjectionMode() const;
    Rect GetScreenBoundingRect(const AABox &bbox);

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;


protected:
    Camera();
    virtual ~Camera();

private:
    float m_orthoHeight  = 25.0f;
    Mesh *p_camMesh = nullptr;

    Color m_clearColor = Color(Color(0.3f), 1);
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;

    ProjectionMode m_projMode = ProjectionMode::Perspective;
};

NAMESPACE_BANG_END

#endif // CAMERA_H
