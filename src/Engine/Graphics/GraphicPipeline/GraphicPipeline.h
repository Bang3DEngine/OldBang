#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#include "Bang/GBuffer.h"
#include "Bang/Renderer.h"

class Mesh;
class Scene;
class Screen;
class Texture;
class Material;
class GLContext;
class GameObject;
class Framebuffer;
class GPPass_RenderLayer;
class TextureUnitManager;
class SelectionFramebuffer;
/**
 * @brief The GraphicPipeline class is the responsible of rendering the scene.
 */
class GraphicPipeline
{
public:
    GraphicPipeline(Screen *screen);
    virtual ~GraphicPipeline();

    static GraphicPipeline* GetActive();

    void RenderScene(Scene *scene, bool inGame);
    void RenderRenderer(Renderer *rend);
    void RenderToScreen(Texture *fullScreenTexture);
    void ApplyScreenPass(ShaderProgram *sp, const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void OnResize(int newWidth, int newHeight);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void SetGBufferAttachmentToBeRendered(GBuffer::AttachmentId attachment);

    GLContext *GetGLContext() const;
    GBuffer *GetGBuffer();
    TextureUnitManager *GetTextureUnitManager() const;
    #ifdef BANG_EDITOR
    SelectionFramebuffer* GetSelectionFramebuffer();
    #endif

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    TextureUnitManager *m_texUnitManager = nullptr;
    GLContext *m_glContext = nullptr;

    GBuffer *m_gbuffer = nullptr;
    #ifdef BANG_EDITOR
    SelectionFramebuffer *m_selectionFB = nullptr;
    #endif

    GPPass_RenderLayer *m_scenePass  = nullptr;
    GPPass_RenderLayer *m_canvasPass = nullptr;
    GPPass_RenderLayer *m_gizmosPass = nullptr;

    #ifdef BANG_EDITOR
    GPPass_RenderLayer *m_sceneSelectionPass  = nullptr;
    GPPass_RenderLayer *m_canvasSelectionPass = nullptr;
    GPPass_RenderLayer *m_gizmosSelectionPass = nullptr;
    #endif

    GBuffer::AttachmentId m_gbufferAttachToBeShown = GBuffer::AttColor;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    Material *m_matSelectionEffectScreen = nullptr;

    void RenderGBuffer(const List<Renderer*> &renderers,
                       const List<GameObject*> &sceneChildren);

    #ifdef BANG_EDITOR
    void RenderSelectionBuffer(const List<Renderer*> &renderers,
                               const List<GameObject*> &sceneChildren,
                               Scene *scene);
    #endif

    void ApplySelectionOutline();
};

#endif // GRAPHICPIPELINE_H
