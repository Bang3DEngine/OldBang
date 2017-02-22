#include "GPPass_DepthLayer.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"

GPPass_DepthLayer::GPPass_DepthLayer(GraphicPipeline *graphicPipeline,
                                     Renderer::DepthLayer depthLayer) :
    GraphicPipelinePass(graphicPipeline),
    m_depthLayer(depthLayer)
{
}

void GPPass_DepthLayer::PrePass(const List<Renderer*> &renderers,
                                const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::PrePass(renderers, sceneChildren);
    p_gbuffer->ClearAllBuffersExceptColor();
}

bool GPPass_DepthLayer::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
           renderer->GetDepthLayer() == m_depthLayer;
}
