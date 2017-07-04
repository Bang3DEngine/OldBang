#include "Bang/G_GPPass_G_Gizmos.h"

#include "Bang/Debug.h"
#include "Bang/Renderer.h"
#include "Bang/G_GBuffer.h"
#include "Bang/GameObject.h"

G_GPPass_G_Gizmos::G_GPPass_G_Gizmos(G_GraphicPipeline *graphicPipeline,
                                 bool depthed, bool overlay,
                                 const List<G_GPPass*> &subPasses)
    : G_GPPass(graphicPipeline, subPasses)
{
    m_depthed = depthed;
    m_overlay = overlay;
}

void G_GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    G_GPPass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(false);

    GL::SetTestDepth(true);
    GL::SetWriteDepth(true);
    //if (m_overlay || !m_depthed) { GL::SetTestDepth(false); }
    for (GameObject *go : sceneChildren)
    {
        go->_OnDrawGizmos(m_depthed, m_overlay);
    }
    // p_gbuffer->ClearStencil();
    p_gbuffer->ClearDepth();
}
