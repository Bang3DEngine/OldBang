#include "GPPass_G_Gizmos.h"

#include "Debug.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "GameObject.h"

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
    if (m_overlay || !m_depthed) { GL::SetTestDepth(false); }
    for (GameObject *go : sceneChildren)
    {
        go->_OnDrawGizmos(m_depthed, m_overlay);
    }
    GL::SetTestDepth(true);
}
