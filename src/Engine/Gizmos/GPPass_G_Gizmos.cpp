#include "Bang/GPPass_G_Gizmos.h"

#include "Bang/Scene.h"
#include "Bang/Renderer.h"
#include "Bang/G_GBuffer.h"
#include "Bang/GameObject.h"

GPPass_G_Gizmos::GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                                 GizmosPassType gizmosPassType,
                                 const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_gizmosPassType = gizmosPassType;
}

void GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(false);

    GL::SetWriteDepth(true);
    GL::SetTestDepth(m_gizmosPassType == GizmosPassType::Depth);
    p_scene->_OnDrawGizmos(m_gizmosPassType);
    // p_gbuffer->ClearStencil();
    p_gbuffer->ClearDepth();
}