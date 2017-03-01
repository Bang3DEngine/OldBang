#include "GPPass_G.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"

GPPass_G::GPPass_G(GraphicPipeline *graphicPipeline,
                   bool receiveLighting,
                   bool transparent)
    : GraphicPipelinePass(graphicPipeline)
{
    m_receiveLighting = receiveLighting;
    m_transparent = transparent;
}

void GPPass_G::InPass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    if (m_transparent) { glDepthMask(false); }

    p_gbuffer->SetStencilTest(false); // Don't want to be filtered by stencil
    // Mark into the stencil (for deferred lighting be applied here)
    p_gbuffer->SetStencilWrite(true);
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }
        rend->Render();
    }
    glDepthMask(true);
}

bool GPPass_G::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
           (m_transparent == renderer->IsTransparent()) &&
           (m_receiveLighting == receivesLighting);
}
