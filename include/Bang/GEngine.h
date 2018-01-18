#ifndef GENGINE_H
#define GENGINE_H

#include "Bang/Rect.h"
#include "Bang/RenderPass.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class GL;
FORWARD class Scene;
FORWARD class Camera;
FORWARD class Texture;
FORWARD class GBuffer;
FORWARD class Renderer;
FORWARD class Texture2D;
FORWARD class Framebuffer;
FORWARD class ShaderProgram;
FORWARD class TextureUnitManager;
FORWARD class SelectionFramebuffer;

class GEngine
{
public:
    GEngine();
    virtual ~GEngine();

    void Init();

    void Render(Renderer *rend);
    void Render(GameObject *go, Camera *camera);
    void RenderToScreen(Camera *cam);
    void RenderToScreen(Texture2D *fullScreenTexture);
    void RenderScreenRect(ShaderProgram *sp,
                          const Rect &destRectMask = Rect::NDCRect);
    void RenderGBufferToScreen(const Rect &gbufferRectMask = Rect::NDCRect,
                         const Rect &destRectMask = Rect::NDCRect);
    void RenderScreenPlane(bool withDepth = false);

    void ApplyStenciledDeferredLightsToGBuffer(GameObject *lightsContainer,
                                               Camera *camera,
                                               const Rect &maskRectNDC = Rect::NDCRect);

    void Resize(int newWidth, int newHeight);

    static Camera *GetActiveCamera();
    static GBuffer *GetActiveGBuffer();
    static SelectionFramebuffer *GetActiveSelectionFramebuffer();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetActive();

private:
    GL *m_gl = nullptr;
    Camera *p_activeCamera = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    RH<Mesh> p_screenPlaneMesh;
    RH<Material> p_renderGBufferToScreenMaterial;

    void RenderToGBuffer(GameObject *go, Camera *camera);
    void RenderToSelectionFramebuffer(GameObject *go, Camera *camera);
    void RenderWithPass(GameObject *go, RenderPass renderPass);
    void RenderWithPassAndMarkStencilForLights(GameObject *go, RenderPass renderPass);

    void SetActiveCamera(Camera *camera);

    static GEngine *s_gEngine;
    static void SetActive(GEngine *gEngine);

    friend class Window;
};

NAMESPACE_BANG_END

#endif // GENGINE_H
