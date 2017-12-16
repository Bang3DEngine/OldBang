#ifndef CAMERA_H
#define CAMERA_H

#include "Bang/Ray.h"
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
    void SetViewportForBlitting() const;
    void SetViewportForRendering() const;

    void BindGBuffer();
    void BindSelectionFramebuffer();

    Ray      FromViewportPointNDCToRay(const Vector2 &vpPointNDC) const;
    Vector2i FromScreenPointToViewportPoint(const Vector2i &screenPoint) const;
    Vector2  FromWorldPointToViewportPointNDC(const Vector3 &worldPosition) const;
    Vector3  FromViewportPointNDCToWorldPoint(const Vector3 &vpPositionNDC) const;
    Vector3  FromViewportPointNDCToWorldPoint(const Vector2 &vpPositionNDC,
                                              float zFromCam) const;

    void SetOrthoHeight(float orthoHeight);
    void SetClearColor(const Color& color);
    void SetFovDegrees(float fovDegrees);
    void SetZNear(float zNear);
    void SetZFar(float zFar);
    void SetProjectionMode(ProjectionMode projMode);
    void SetViewportRect(const Rect &viewportRectNDC);

    const Color& GetClearColor() const;
    float GetOrthoWidth() const;
    float GetOrthoHeight() const;
    float GetFovDegrees() const;
    float GetZNear() const;
    float GetZFar() const;
    Matrix4 GetViewMatrix() const;
    Matrix4 GetProjectionMatrix() const;
    ProjectionMode GetProjectionMode() const;
    Rect GetScreenBoundingRect(const AABox &bbox);
    GameObject *GetGameObjectToRender() const;
    Rect GetViewportScreenRect() const;
    Rect GetViewportScreenRectNDC() const;
    const Rect& GetViewportRectNDC() const;
    GBuffer *GetGBuffer() const;
    SelectionFramebuffer *GetSelectionFramebuffer() const;

    static Camera *GetActive();

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

    GameObject *p_gameObjectToRender = nullptr;

    Color m_clearColor = Color(Color(0.3f), 1);
    float m_orthoHeight  = 25.0f;
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;
    Rect m_viewportRectNDC = Rect::NDCRect;
    ProjectionMode m_projMode = ProjectionMode::Perspective;

    mutable Recti m_latestViewportRect = Recti::Zero;

    void SetGameObjectToRender(GameObject *go);

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // CAMERA_H
