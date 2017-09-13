#ifndef GENGINE_H
#define GENGINE_H

#include "Bang/Rect.h"

NAMESPACE_BANG_BEGIN

FORWARD class GL;
FORWARD class Scene;
FORWARD class Camera;
FORWARD class Texture;
FORWARD class GBuffer;
FORWARD class Renderer;
FORWARD class Framebuffer;
FORWARD class ShaderProgram;
FORWARD class RenderSurface;
FORWARD class TextureUnitManager;
FORWARD class SelectionFramebuffer;

class GEngine
{
public:
    GEngine();
    virtual ~GEngine();

    void Render(Scene *scene, Camera *camera = nullptr);
    void RenderCurrentScene(Camera *camera = nullptr);
    void RenderToScreen(Texture *fullScreenTexture);
    void ApplyScreenPass(ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void Render(Renderer *rend);

    void ApplyDeferredLights(Camera *camera, Renderer *rend = nullptr);

    static void SetCurrentScene(Scene *scene);
    static void SetCurrentRenderSurface(RenderSurface *renderSurface);

    static Scene *GetCurrentScene();
    static GBuffer *GetCurrentGBuffer();
    static SelectionFramebuffer *GetCurrentSelectionFramebuffer();
    static RenderSurface *GetCurrentRenderSurface();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetInstance();

private:
    GL *m_gl = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    Scene *p_currentScene = nullptr;
    RenderSurface *p_currentRenderSurface = nullptr;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    void RenderCurrentSceneToGBuffer(Camera *camera);
    void RenderCurrentSceneToSelectionFramebuffer(Camera *camera);
};

NAMESPACE_BANG_END

#endif // GENGINE_H
