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
class GPPass_RenderLayer;
class G_TextureUnitManager;
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
    void ApplyScreenPass(G_ShaderProgram *sp, const Rect &mask = Rect::ScreenRect);
    void RenderScreenPlane();

    void Render(Renderer *renderer) const;
    void OnResize(int newWidth, int newHeight);

    void ApplySPEffectToRenderer(const Renderer *renderer, Material *mat);

    void SetGBufferAttachmentToBeRendered(G_GBuffer::AttachmentId attachment);

    GLContext *GetGLContext() const;
    G_GBuffer *GetGBuffer();
    G_TextureUnitManager *GetTextureUnitManager() const;

    /**
     * @brief Apply all the scene lights over the current gbuffer.
     */
    void ApplyDeferredLights(Renderer *rend = nullptr);

private:
    G_TextureUnitManager *m_texUnitManager = nullptr;
    GLContext *m_glContext = nullptr;

    G_GBuffer *m_gbuffer = nullptr;

    GPPass_RenderLayer *m_scenePass  = nullptr;
    GPPass_RenderLayer *m_canvasPass = nullptr;

    G_GBuffer::AttachmentId m_gbufferAttachToBeShown = G_GBuffer::AttColor;

    // Rendering for the Game Screen or the Scene screen
    bool m_renderingInGame = false;

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_screenPlaneMesh = nullptr;

    Scene *p_scene = nullptr;

    void RenderGBuffer(const List<Renderer*> &renderers,
                       const List<GameObject*> &sceneChildren);
};

#endif // GRAPHICPIPELINE_H
