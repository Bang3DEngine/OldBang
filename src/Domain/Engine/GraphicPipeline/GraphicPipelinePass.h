#ifndef GRAPHICPIPELINEPASS_H
#define GRAPHICPIPELINEPASS_H

#include "List.h"

class Scene;
class GBuffer;
class Renderer;
class GameObject;
class GraphicPipeline;
class SelectionFramebuffer;
class GraphicPipelinePass
{
public:
    GraphicPipelinePass(GraphicPipeline *graphicPipeline);
    virtual ~GraphicPipelinePass();

    virtual void PrePass(const List<Renderer*> &renderers,
                         const List<GameObject*> &sceneChildren);
    virtual void InPass(const List<Renderer*> &renderers,
                        const List<GameObject*> &sceneChildren);
    virtual void PostPass(const List<Renderer*> &renderers,
                          const List<GameObject*> &sceneChildren);

    virtual void Pass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren);

    void AddSubPass(GraphicPipelinePass *subPass);
    virtual bool CanRender(const Renderer *renderer) const;

protected:
    Scene *p_scene = nullptr;
    GBuffer *p_gbuffer = nullptr;
    GraphicPipeline *p_graphicPipeline = nullptr;
    GraphicPipelinePass *p_parentPass = nullptr;
    SelectionFramebuffer *p_selectionFramebuffer = nullptr;
    List<GraphicPipelinePass*> m_subPasses;

};

#endif // GRAPHICPIPELINEPASS_H
