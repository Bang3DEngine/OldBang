#ifndef GRAPHICPIPELINEPASS_H
#define GRAPHICPIPELINEPASS_H

#include "Bang/List.h"

class Scene;
class G_GBuffer;
class Renderer;
class GameObject;
class G_GraphicPipeline;
class SelectionFramebuffer;
class G_GPPass
{
public:
    G_GPPass(G_GraphicPipeline *graphicPipeline,
             const List<G_GPPass*> &subPasses = {});
    virtual ~G_GPPass();

    virtual void PrePass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren);
    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren);
    virtual void PostPass(const List<Renderer*> &renderers,
                          const List<GameObject*> &sceneChildren);

    virtual void Pass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren);

    void AddSubPass(G_GPPass *subPass);
    virtual bool CanRender(const Renderer *renderer) const;

protected:
    Scene *p_scene = nullptr;
    G_GBuffer *p_gbuffer = nullptr;
    G_GPPass *p_parentPass = nullptr;
    G_GraphicPipeline *p_graphicPipeline = nullptr;

    List<G_GPPass*> m_subPasses;

    #ifdef BANG_EDITOR
    SelectionFramebuffer *p_selectionG_Framebuffer = nullptr;
    #endif

};

#endif // GRAPHICPIPELINEPASS_H
