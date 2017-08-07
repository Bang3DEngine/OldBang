#include "Bang/GPPass_G_Gizmos.h"

#include "Bang/Scene.h"
#include "Bang/Renderer.h"
#include "Bang/G_GBuffer.h"
#include "Bang/GameObject.h"

GPPass_G_Gizmos::GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                                 const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
}

void GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(false);

    GL::SetWriteDepth(true);
    p_scene->RenderGizmos();
    // p_gbuffer->ClearStencil();
    p_gbuffer->ClearDepth();
}
