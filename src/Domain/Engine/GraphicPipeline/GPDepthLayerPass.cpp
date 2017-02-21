#include "GPDepthLayerPass.h"

#include "Scene.h"
#include "GBuffer.h"

GPDepthLayerPass::GPDepthLayerPass(GraphicPipeline *graphicPipeline,
                                   Renderer::DepthLayer depthLayer) :
    GraphicPipelinePass(graphicPipeline),
    m_depthLayer(depthLayer)
{
}

void GPDepthLayerPass::PrePass(const List<Renderer*> &renderers)
{
    GraphicPipelinePass::PrePass(renderers);
    p_gbuffer->ClearAllBuffersExceptColor();
}

bool GPDepthLayerPass::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
           renderer->GetDepthLayer() == m_depthLayer;
}
