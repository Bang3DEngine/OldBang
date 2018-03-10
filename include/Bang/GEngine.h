#ifndef GENGINE_H
#define GENGINE_H

#include "Bang/AARect.h"
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
    void RenderRaw(Renderer *rend);
    void Render(GameObject *go, Camera *camera);
    void RenderTexture(Texture2D *texture);
    void RenderWithPass(GameObject *go, RenderPass renderPass);
    void RenderWithPassRaw(GameObject *go, RenderPass renderPass);
    void RenderViewportRect(ShaderProgram *sp,
                            const AARect &destRectMask = AARect::NDCRect);
    void RenderViewportPlane();

    void ApplyStenciledDeferredLightsToGBuffer(GameObject *lightsContainer,
                                               Camera *camera,
                                               const AARect &maskRectNDC = AARect::NDCRect);

    static GBuffer *GetActiveGBuffer();
    static Camera *GetActiveRenderingCamera();
    static SelectionFramebuffer *GetActiveSelectionFramebuffer();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetActive();

private:
    GL *m_gl = nullptr;
    Camera *p_currentRenderingCamera = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    RH<Mesh> p_windowPlaneMesh;
    RH<Material> p_renderTextureToViewportMaterial;

    void RenderShadowMaps(GameObject *go);
    void RenderToGBuffer(GameObject *go, Camera *camera);
    void RenderToSelectionFramebuffer(GameObject *go, Camera *camera);
    void RenderWithPassAndMarkStencilForLights(GameObject *go, RenderPass renderPass);

    void SetActiveRenderingCamera(Camera *camera);

    static GEngine *s_gEngine;
    static void SetActive(GEngine *gEngine);

    friend class Window;
};

NAMESPACE_BANG_END

#endif // GENGINE_H
