#ifndef GENGINE_H
#define GENGINE_H

#include "Bang/GL.h"
#include "Bang/Rect.h"

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

    void Render(Renderer *rend);
    void Render(Scene *scene);
    void RenderCurrentScene(Camera *camera);
    void RenderToScreen(Texture *fullScreenTexture);
    void ApplyScreenPass(ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);
    void ApplyDeferredLights(Camera *camera, Renderer *rend = nullptr);

    void Resize(int newWidth, int newHeight);
    static void SetCurrentScene(Scene *scene);

    static Scene *GetCurrentScene();
    static GBuffer *GetGBuffer();
    static SelectionFramebuffer *GetSelectionFramebuffer();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetInstance();

private:
    GL *m_gl = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    Scene *p_currentScene = nullptr;
    GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionFramebuffer = nullptr;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    void RenderCurrentSceneToGBuffer(Camera *camera);
    void RenderCurrentSceneToSelectionFramebuffer(Camera *camera);
};

NAMESPACE_BANG_END

#endif // GENGINE_H
