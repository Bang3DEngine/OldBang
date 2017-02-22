#include "GPPass_G_OpaqueSP.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"

GPPass_G_OpaqueSP::GPPass_G_OpaqueSP(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_G_OpaqueSP::InPass(const List<Renderer*> &renderers,
                               const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

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

bool GPPass_G_OpaqueSP::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            !renderer->IsTransparent() &&
            receivesLighting;
}
