#include "GPPass_G_Gizmos.h"

#include "Debug.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "GameObject.h"

GPPass_G_Gizmos::GPPass_G_Gizmos(GraphicPipeline *graphicPipeline,
                                 bool depthed, bool overlay)
    : GraphicPipelinePass(graphicPipeline)
{
    m_depthed = depthed;
    m_overlay = overlay;
}

void GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(false);

    glDepthFunc(GL_LESS);
    if (m_overlay || !m_depthed) { glDepthFunc(GL_ALWAYS); }
    for (GameObject *go : sceneChildren)
    {
        go->_OnDrawGizmos(m_depthed, m_overlay);
    }
    glDepthFunc(GL_LESS);
}
