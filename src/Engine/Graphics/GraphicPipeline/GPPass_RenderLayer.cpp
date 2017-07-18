#include "Bang/GPPass_RenderLayer.h"

#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"

GPPass_RenderLayer::GPPass_RenderLayer(GraphicPipeline *graphicPipeline,
                                     Renderer::RenderLayer depthLayer,
                                     const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_renderLayer = depthLayer;
}

bool GPPass_RenderLayer::CanRender(const Renderer *renderer) const
{
    return GPPass::CanRender(renderer) &&
           renderer->GetRenderLayer() == m_renderLayer;
}

Renderer::RenderLayer GPPass_RenderLayer::GetRenderLayer() const
{
    return m_renderLayer;
}
