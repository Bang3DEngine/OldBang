#include "GPOpaquePass.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "GraphicPipeline.h"

GPOpaquePass::GPOpaquePass(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPOpaquePass::InPass(const List<Renderer*> &renderers)
{
    GraphicPipelinePass::InPass(renderers);

    p_gbuffer->SetAllDrawBuffers();
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }

        // Don't want to be filtered by the stencil
        p_gbuffer->SetStencilTest(false);

        // We are going to mark into the stencil
        // (to later let the deferred lighting be applied here)
        p_gbuffer->SetStencilWrite(true);

        rend->Render();
    }
}

void GPOpaquePass::PostPass(const List<Renderer *> &renderers)
{
    GraphicPipelinePass::PostPass(renderers);
}

bool GPOpaquePass::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            !renderer->IsTransparent();
}
