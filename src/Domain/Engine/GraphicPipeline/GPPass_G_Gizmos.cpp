#include "GPPass_G_Gizmos.h"

#include "Debug.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "GameObject.h"

GPPass_G_Gizmos::GPPass_G_Gizmos(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_G_Gizmos::InPass(const List<Renderer*> &renderers,
                             const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    glDepthFunc(GL_ALWAYS); // TODO: Must put this because clear depth is not working
    p_gbuffer->SetAllDrawBuffers();
    p_gbuffer->SetStencilTest(false);
    p_gbuffer->SetStencilWrite(false);

    for (GameObject *go : sceneChildren)
    {
        go->_OnDrawGizmos();
    }

    p_gbuffer->ClearDepth();

    for (GameObject *go : sceneChildren)
    {
        go->_OnDrawGizmosOverlay();
    }
    glDepthFunc(GL_LESS);
}
