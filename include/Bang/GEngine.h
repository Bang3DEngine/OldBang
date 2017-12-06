#ifndef GENGINE_H
#define GENGINE_H

#include "Bang/GL.h"
#include "Bang/Rect.h"
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

    void Render(Scene *scene);
    void Render(Renderer *rend);
    void Render(GameObject *go, Camera *camera);
    void RenderToScreen(Camera *cam);
    void RenderToScreen(Texture2D *fullScreenTexture);
    void ApplyScreenPass(ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRectNDC);
    void RenderScreenPlane(bool withDepth = false);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);
    void ApplyDeferredLights(GameObject *lightsContainer,
                             Renderer *lightReceiver,
                             Camera *camera);
    void ApplyDeferredLights(GameObject *lightsContainer,
                             GameObject *lightReceiver,
                             Camera *camera);
    void ApplyDeferredLightsToGBuffer(GameObject *lightsContainer,
                                      Camera *camera,
                                      const Rect &maskRectNDC);

    void Resize(int newWidth, int newHeight);

    static GBuffer *GetCurrentGBuffer();
    static SelectionFramebuffer *GetCurrentSelectionFramebuffer();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetActive();

private:
    GL *m_gl = nullptr;
    Camera *p_activeCamera = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    RH<Mesh> p_screenPlaneMesh;
    RH<Material> p_renderGBufferToScreenMaterial;

    void SetActiveCamera(Camera *camera);
    void RenderToGBuffer(GameObject *go, Camera *camera);
    void RenderToSelectionFramebuffer(GameObject *go, Camera *camera);

    static GEngine *s_gEngine;
    static void SetActive(GEngine *gEngine);

    friend class Window;
};

NAMESPACE_BANG_END

#endif // GENGINE_H
