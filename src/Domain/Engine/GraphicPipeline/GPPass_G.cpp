#include "GPPass_G.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"

GPPass_G::GPPass_G(GraphicPipeline *graphicPipeline,
                   bool receiveLighting, bool transparent,
                   const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_receiveLighting = receiveLighting;
    m_transparent = transparent;
}

void GPPass_G::InPass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    if (m_transparent) { GL::SetWriteDepth(false); }

    p_gbuffer->SetStencilTest(false); // Don't want to be filtered by stencil
    // Mark into the stencil (for deferred lighting be applied here)
    p_gbuffer->SetStencilWrite(true);
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }
        rend->Render();
    }
    GL::SetWriteDepth(true);
}

bool GPPass_G::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    return GPPass::CanRender(renderer) &&
           (m_transparent == renderer->IsTransparent()) &&
           (m_receiveLighting == receivesLighting);
}
