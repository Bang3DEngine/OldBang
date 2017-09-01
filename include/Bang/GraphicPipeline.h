#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Bang/Rect.h"

FORWARD class GL;
FORWARD class Scene;
FORWARD class G_Screen;
FORWARD class Renderer;
FORWARD class G_Texture;
FORWARD class G_GBuffer;
FORWARD class G_Framebuffer;
FORWARD class G_ShaderProgram;
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

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(G_Texture *fullScreenTexture);
    void ApplyScreenPass(G_ShaderProgram *sp,
                         const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void Render(Renderer *rend);
    void OnResize(int newWidth, int newHeight);

    GL *GetGL() const;
    G_GBuffer *GetGBuffer();
    SelectionFramebuffer *GetSelectionFramebuffer();
    G_TextureUnitManager *GetTextureUnitManager() const;
    static GraphicPipeline* GetActive();

    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    G_TextureUnitManager *m_texUnitManager = nullptr;
    GL *m_gl = nullptr;

    G_GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionFB = nullptr;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    void RenderGBuffer(Scene *scene);
    void RenderSelectionBuffer(Scene *scene);
};

#endif // GRAPHICPIPELINE_H
