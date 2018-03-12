#ifndef CAMERA_H
#define CAMERA_H

#include "Bang/Ray.h"
#include "Bang/Set.h"
#include "Bang/Color.h"
#include "Bang/AARect.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class Quad;
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
    void BindViewportForBlitting() const;
    void BindViewportForRendering() const;

    Ray      FromViewportPointNDCToRay(const Vector2 &vpPointNDC) const;
    Vector2i FromWindowPointToViewportPoint(const Vector2i &winPoint) const;
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
    void SetViewportRect(const AARect &viewportRectNDC);
    void AddRenderPass(RenderPass renderPass);
    void RemoveRenderPass(RenderPass renderPass);
    void SetRenderSelectionBuffer(bool renderSelectionBuffer);

    const Color& GetClearColor() const;
    float GetOrthoWidth() const;
    float GetOrthoHeight() const;
    float GetFovDegrees() const;
    float GetZNear() const;
    float GetZFar() const;
    bool MustRenderPass(RenderPass renderPass) const;
    const Set<RenderPass>& GetRenderPassMask() const;
    Matrix4 GetViewMatrix() const;
    bool GetRenderSelectionBuffer() const;
    Matrix4 GetProjectionMatrix() const;
    ProjectionMode GetProjectionMode() const;
    AARect GetViewportBoundingRect(const AABox &bbox);
    AARect GetViewportRectInWindow() const;
    AARect GetViewportRectNDCInWindow() const;
    const AARect& GetViewportRectNDC() const;
    GBuffer *GetGBuffer() const;
    SelectionFramebuffer *GetSelectionFramebuffer() const;

    Quad GetNearQuad()  const;
    Quad GetFarQuad()   const;
    Quad GetLeftQuad()  const;
    Quad GetRightQuad() const;
    Quad GetTopQuad()   const;
    Quad GetBotQuad()   const;

    static Camera *GetActive();

    // Component
    void OnRender(RenderPass rp) override;

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

    Set<RenderPass> m_renderPassMask;
    bool m_renderSelectionBuffer = false;

    Color m_clearColor = Color(Color(0.3f), 1);
    float m_orthoHeight  = 25.0f;
    float m_fovDegrees = 60.0f;
    float m_zNear = 0.1f;
    float m_zFar = 100.0f;
    AARect m_viewportRectNDC = AARect::NDCRect;
    ProjectionMode m_projMode = ProjectionMode::Perspective;

    friend class Scene;
};

NAMESPACE_BANG_END

#endif // CAMERA_H
