#include "Bang/G_GPPass_G.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/G_GraphicPipeline.h"

G_GPPass_G::G_GPPass_G(G_GraphicPipeline *graphicPipeline,
                   bool receiveLighting, bool transparentPass,
                   const List<G_GPPass*> &subPasses)
    : G_GPPass(graphicPipeline, subPasses)
{
    m_receiveLighting = receiveLighting;
    m_transparentPass = transparentPass;
}

void G_GPPass_G::InPass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren)
{
    G_GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    //if (m_transparentPass) { GL::SetWriteDepth(false); }

    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(true);
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }
        if (rend->IsTransparent())
        {
            p_gbuffer->PrepareColorReadBuffer();
            p_gbuffer->SetAllDrawBuffers();
        }
        rend->Render();
    }
    //if (m_transparentPass) { GL::SetWriteDepth(true); }
}

bool G_GPPass_G::CanRender(const Renderer *renderer) const
{
    Material *rendMaterial = renderer->GetMaterial();
    bool receivesLighting = rendMaterial && rendMaterial->ReceivesLighting();
    return G_GPPass::CanRender(renderer) &&
           (m_transparentPass == renderer->IsTransparent()) &&
           (m_receiveLighting == receivesLighting);
}
