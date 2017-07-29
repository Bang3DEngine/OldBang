#ifndef GRAPHICPIPELINEPASS_H
#define GRAPHICPIPELINEPASS_H

#include "Bang/List.h"

FORWARD class Scene;
FORWARD class Renderer;
FORWARD class G_GBuffer;
FORWARD class GraphicPipeline;

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
    G_GBuffer *p_gbuffer = nullptr;
    GPPass *p_parentPass = nullptr;
    GraphicPipeline *p_graphicPipeline = nullptr;

    List<GPPass*> m_subPasses;
};

#endif // GRAPHICPIPELINEPASS_H
