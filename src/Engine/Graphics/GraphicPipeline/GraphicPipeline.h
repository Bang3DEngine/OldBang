#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Bang/Rect.h"

FORWARD class Scene;
FORWARD class G_Screen;
FORWARD class Renderer;
FORWARD class G_Texture;
FORWARD class GLContext;
FORWARD class G_GBuffer;
FORWARD class G_Framebuffer;
FORWARD class G_ShaderProgram;
FORWARD class GPPass_RenderLayer;
FORWARD class G_TextureUnitManager;
FORWARD class SelectionFramebuffer;

/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class GraphicPipeline
{
public:
    GraphicPipeline(G_Screen *screen);
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(G_Texture *fullScreenTexture);
    void ApplyScreenPass(G_ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void Render(Renderer *renderer) const;
    void OnResize(int newWidth, int newHeight);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    GLContext *GetGLContext() const;
    G_GBuffer *GetGBuffer();
    SelectionFramebuffer *GetSelectionFramebuffer();
    G_TextureUnitManager *GetTextureUnitManager() const;

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    G_TextureUnitManager *m_texUnitManager = nullptr;
    GLContext *m_glContext = nullptr;

    G_GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionFB = nullptr;

    GPPass_RenderLayer *m_scenePass  = nullptr;
    GPPass_RenderLayer *m_canvasPass = nullptr;
    GPPass_RenderLayer *m_gizmosPass = nullptr;
    GPPass_RenderLayer *m_sceneSelectionPass  = nullptr;
    GPPass_RenderLayer *m_canvasSelectionPass = nullptr;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    void RenderGBuffer(const List<Renderer*> &renderers,
                       const List<GameObject*> &sceneChildren);
    void RenderSelectionBuffer(const List<Renderer *> &renderers,
                               const List<GameObject *> &sceneChildren,
                               Scene *scene);
};

#endif // GRAPHICPIPELINE_H
