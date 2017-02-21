#ifndef GRAPHICPIPELINEPASS_H
#define GRAPHICPIPELINEPASS_H

#include "List.h"

class Scene;
class GBuffer;
class Renderer;
class GraphicPipeline;
class GraphicPipelinePass
{
public:
    GraphicPipelinePass(GraphicPipeline *graphicPipeline);
    virtual ~GraphicPipelinePass();

    virtual void PrePass(const List<Renderer*> &renderers);
    virtual void InPass(const List<Renderer*> &renderers);
    virtual void PostPass(const List<Renderer*> &renderers);

    virtual void Pass(const List<Renderer*> &renderers);

    void AddSubPass(GraphicPipelinePass *subPass);
    virtual bool CanRender(const Renderer *renderer) const;

protected:
    Scene *p_scene = nullptr;
    GBuffer *p_gbuffer = nullptr;
    GraphicPipeline *p_graphicPipeline = nullptr;
    GraphicPipelinePass *p_parentPass = nullptr;
    List<GraphicPipelinePass*> m_subPasses;

};

#endif // GRAPHICPIPELINEPASS_H
