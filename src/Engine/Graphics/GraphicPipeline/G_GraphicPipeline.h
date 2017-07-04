#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Bang/Renderer.h"
#include "Bang/G_GBuffer.h"

class Mesh;
class Scene;
class G_Screen;
class G_Texture;
class Material;
class GLContext;
class GameObject;
class G_Framebuffer;
class G_GPPass_RenderLayer;
class G_TextureUnitManager;
class SelectionFramebuffer;
/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class G_GraphicPipeline
{
public:
    G_GraphicPipeline(G_Screen *screen);
    virtual ~G_GraphicPipeline();

    static G_GraphicPipeline* GetActive();

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(G_Texture *fullScreenTexture);
    void ApplyScreenPass(G_ShaderProgram *sp, const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void OnResize(int newWidth, int newHeight);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void SetG_GBufferAttachmentToBeRendered(G_GBuffer::AttachmentId attachment);

    GLContext *GetGLContext() const;
    G_GBuffer *GetG_GBuffer();
    G_TextureUnitManager *GetTextureUnitManager() const;
    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer();
    #endif

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    G_TextureUnitManager *m_texUnitManager = nullptr;
    GLContext *m_glContext = nullptr;

    G_GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    G_GPPass_RenderLayer *m_scenePass  = nullptr;
    G_GPPass_RenderLayer *m_canvasPass = nullptr;
    G_GPPass_RenderLayer *m_gizmosPass = nullptr;

    #ifdef BANG_EDITOR
    G_GPPass_RenderLayer *m_sceneSelectionPass  = nullptr;
    G_GPPass_RenderLayer *m_canvasSelectionPass = nullptr;
    G_GPPass_RenderLayer *m_gizmosSelectionPass = nullptr;
    #endif

    G_GBuffer::AttachmentId m_gbufferAttachToBeShown = G_GBuffer::AttColor;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    Material *m_matSelectionEffectScreen = nullptr;

    void RenderG_GBuffer(const List<Renderer*> &renderers,
                       const List<GameObject*> &sceneChildren);

    #ifdef BANG_EDITOR
    void RenderSelectionBuffer(const List<Renderer*> &renderers,
                               const List<GameObject*> &sceneChildren,
                               Scene *scene);
    #endif

    void ApplySelectionOutline();
};

#endif // GRAPHICPIPELINE_H
