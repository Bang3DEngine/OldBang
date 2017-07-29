#include "Bang/GPPass_SP_DeferredLights.h"

#include "Bang/G_GBuffer.h"
#include "Bang/GraphicPipeline.h"

GPPass_SP_DeferredLights::GPPass_SP_DeferredLights(
                            GraphicPipeline *graphicPipeline,
                            const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
}

void GPPass_SP_DeferredLights::InPass(const List<Renderer *> &renderers,
                                      const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);
    p_graphicPipeline->ApplyDeferredLights();

    // TODO: will have to change this in a future and maybe use clear stencil as
    // a separate GPPass
    p_graphicPipeline->GetGBuffer()->ClearStencil();
}

