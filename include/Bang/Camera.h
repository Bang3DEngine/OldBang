#ifndef CAMERA_H
#define CAMERA_H

#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GBuffer;
FORWARD class Texture2D;
FORWARD class ShaderProgram;
FORWARD class SelectionFramebuffer;

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
    virtual void UnBind() const;
    void BindGBuffer();
    void BindSelectionFramebuffer();

    Vector2i FromScreenPointToViewport(const Vector2i &screenPointPx);
    Vector2 FromWorldPointToScreenNDC(const Vector3 &position);
    Vector3 FromScreenNDCPointToWorld(const Vector2 &screenNDCPos, float zFromCam);

    void SetOrthoHeight(float orthoHeight);
    void SetClearColor(const Color& color);
    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);
    void SetRenderTexture(Texture2D *renderTexture);
    void SetProjectionMode(ProjectionMode projMode);
    void SetViewportRect(const Rect &viewportRectNDC);

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
    Texture2D *GetRenderTexture() const;
    GameObject *GetGameObjectToRender() const;
    const Rect& GetViewportRect() const;
    GBuffer *GetGBuffer() const;
    SelectionFramebuffer *GetSelectionFramebuffer() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Camera();
    virtual ~Camera();

private:
    GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionFramebuffer = nullptr;

    Texture2D *p_renderTexture = nullptr;
    GameObject *p_gameObjectToRender = nullptr;

    Color m_clearColor = Color(Color(0.3f), 1);
    float m_orthoHeight  = 25.0f;
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;
    Rect m_viewportRect = Rect::ScreenRect;
    ProjectionMode m_projMode = ProjectionMode::Perspective;

    mutable Recti m_latestViewportRect = Recti::Zero;

    void SetGameObjectToRender(GameObject *go);

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // CAMERA_H
