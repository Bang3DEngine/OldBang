#include "GPPass_SP_DeferredLights.h"

#include "GraphicPipeline.h"

GPPass_SP_DeferredLights::GPPass_SP_DeferredLights(
                                              GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_SP_DeferredLights::InPass(const List<Renderer *> &renderers,
                                      const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);
    p_graphicPipeline->ApplyDeferredLights();
}

