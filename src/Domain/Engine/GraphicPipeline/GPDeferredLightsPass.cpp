#include "GPDeferredLightsPass.h"

#include "GraphicPipeline.h"

GPDeferredLightsPass::GPDeferredLightsPass(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPDeferredLightsPass::InPass(const List<Renderer *> &renderers)
{
    GraphicPipelinePass::InPass(renderers);
    p_graphicPipeline->ApplyDeferredLights();
}

