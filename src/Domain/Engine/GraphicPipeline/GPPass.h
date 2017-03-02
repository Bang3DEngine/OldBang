#ifndef GRAPHICPIPELINEPASS_H
#define GRAPHICPIPELINEPASS_H

#include "List.h"

class Scene;
class GBuffer;
class Renderer;
class GameObject;
class GraphicPipeline;
class SelectionFramebuffer;
class GPPass
{
public:
    GPPass(GraphicPipeline *graphicPipeline,
                        const List<GPPass*> &subPasses = {});
    virtual ~GPPass();

    virtual void PrePass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren);
    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren);
    virtual void PostPass(const List<Renderer*> &renderers,
                          const List<GameObject*> &sceneChildren);

    virtual void Pass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren);

    void AddSubPass(GPPass *subPass);
    virtual bool CanRender(const Renderer *renderer) const;

protected:
    Scene *p_scene = nullptr;
    GBuffer *p_gbuffer = nullptr;
    GPPass *p_parentPass = nullptr;
    GraphicPipeline *p_graphicPipeline = nullptr;

    List<GPPass*> m_subPasses;

    #ifdef BANG_EDITOR
    SelectionFramebuffer *p_selectionFramebuffer = nullptr;
    #endif

};

#endif // GRAPHICPIPELINEPASS_H
