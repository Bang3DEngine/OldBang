#include "Bang/GPPass_G_Gizmos.h"

#include "Bang/Debug.h"
#include "Bang/GBuffer.h"
#include "Bang/Renderer.h"
#include "Bang/GameObject.h"

GPPass_G_Gizmos::GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                                 bool depthed, bool overlay,
                                 const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_depthed = depthed;
    m_overlay = overlay;
}

void GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

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
