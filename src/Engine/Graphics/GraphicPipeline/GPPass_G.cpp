#include "Bang/GPPass_G.h"

#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/GraphicPipeline.h"

GPPass_G::GPPass_G(GraphicPipeline *graphicPipeline,
                   ReceiveLightPass receiveLighting,
                   const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_receiveLighting = receiveLighting;
}

void GPPass_G::InPass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();

    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(true);
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }
        p_graphicPipeline->Render(rend);
    }
}

bool GPPass_G::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    bool canRender =
          (m_receiveLighting == ReceiveLightPass::Yes && receivesLighting) ||
          (m_receiveLighting == ReceiveLightPass::No  && receivesLighting) ||
           m_receiveLighting == ReceiveLightPass::Both;
    return GPPass::CanRender(renderer) && canRender;
}
