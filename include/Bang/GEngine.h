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

    void Init();

    void Render(Scene *scene);
    void Render(Renderer *rend);
    void Render(GameObject *go, Camera *camera);
    void RenderToScreen(Camera *cam);
    void RenderToScreen(Texture *fullScreenTexture);
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

    void _BindCamera(Camera *cam);
    static void BindCamera(Camera *cam);

    static GBuffer *GetCurrentGBuffer();
    static Camera *GetBoundCamera();
    static SelectionFramebuffer *GetCurrentSelectionFramebuffer();

    GL *GetGL() const;
    TextureUnitManager *GetTextureUnitManager() const;

    static GEngine* GetInstance();

private:
    GL *m_gl = nullptr;
    TextureUnitManager *m_texUnitManager = nullptr;

    Camera *p_boundCamera = nullptr;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    void RenderToGBuffer(GameObject *go, Camera *camera);
    void RenderToSelectionFramebuffer(GameObject *go, Camera *camera);
};

NAMESPACE_BANG_END

#endif // GENGINE_H
