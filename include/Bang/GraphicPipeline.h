#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Bang/Rect.h"

NAMESPACE_BANG_BEGIN

FORWARD class GL;
FORWARD class Scene;
FORWARD class Screen;
FORWARD class Renderer;
FORWARD class Texture;
FORWARD class GBuffer;
FORWARD class Framebuffer;
FORWARD class ShaderProgram;
FORWARD class TextureUnitManager;
FORWARD class SelectionFramebuffer;

/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class GraphicPipeline
{
public:
    GraphicPipeline(Screen *screen);
    virtual ~GraphicPipeline();

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(Texture *fullScreenTexture);
    void ApplyScreenPass(ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void Render(Renderer *rend);
    void OnResize(int newWidth, int newHeight);

    GL *GetGL() const;
    GBuffer *GetGBuffer();
    SelectionFramebuffer *GetSelectionFramebuffer();
    TextureUnitManager *GetTextureUnitManager() const;
    static GraphicPipeline* GetActive();

    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    TextureUnitManager *m_texUnitManager = nullptr;
    GL *m_gl = nullptr;

    GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionFB = nullptr;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    void RenderGBuffer(Scene *scene);
    void RenderSelectionBuffer(Scene *scene);
};

NAMESPACE_BANG_END

#endif // GRAPHICPIPELINE_H
