#include "GPPass_G_OpaqueNoSP.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"

GPPass_G_OpaqueNoSP::GPPass_G_OpaqueNoSP(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_G_OpaqueNoSP::InPass(const List<Renderer*> &renderers,
                                 const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }

        // Don't care about the stencil here
        p_gbuffer->SetStencilTest(false);
        p_gbuffer->SetStencilWrite(false);
        rend->Render();
    }
}

bool GPPass_G_OpaqueNoSP::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            !renderer->IsTransparent() &&
            !receivesLighting;
}
