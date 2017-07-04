#include "Bang/G_GPPass_SP_DeferredLights.h"

#include "Bang/G_GraphicPipeline.h"

G_GPPass_SP_DeferredLights::G_GPPass_SP_DeferredLights(
                            G_GraphicPipeline *graphicPipeline,
                            const List<G_GPPass*> &subPasses)
    : G_GPPass(graphicPipeline, subPasses)
{
}

void G_GPPass_SP_DeferredLights::InPass(const List<Renderer *> &renderers,
                                      const List<GameObject*> &sceneChildren)
{
    G_GPPass::InPass(renderers, sceneChildren);
    p_graphicPipeline->ApplyDeferredLights();

    // TODO: will have to change this in a future and maybe use clear stencil as
    // a separate GPPass
    p_graphicPipeline->GetG_GBuffer()->ClearStencil();
}

