#include "GraphicPipelinePass.h"

#include "Scene.h"
#include "GBuffer.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"

void GraphicPipelinePass::PrePass(const List<Renderer*> &renderers,
                                  const List<GameObject*> &sceneChildren)
{
}
void GraphicPipelinePass::InPass(const List<Renderer*> &renderers,
                                 const List<GameObject*> &sceneChildren)
{
}
void GraphicPipelinePass::PostPass(const List<Renderer*> &renderers,
                                   const List<GameObject*> &sceneChildren)
{
}

void GraphicPipelinePass::Pass(const List<Renderer*> &renderers,
                               const List<GameObject*> &sceneChildren)
{
    p_scene = SceneManager::GetActiveScene();

    PrePass(renderers, sceneChildren);
    InPass(renderers, sceneChildren);

    for (GraphicPipelinePass *gpSubPass : m_subPasses)
    {
        gpSubPass->Pass(renderers, sceneChildren);
    }

    PostPass(renderers, sceneChildren);
}

void GraphicPipelinePass::AddSubPass(GraphicPipelinePass *subPass)
{
    m_subPasses.PushBack(subPass);
    subPass->p_parentPass = this;
}

bool GraphicPipelinePass::CanRender(const Renderer *renderer) const
{
    return true;
}

GraphicPipelinePass::GraphicPipelinePass(GraphicPipeline *graphicPipeline) :
    p_graphicPipeline(graphicPipeline),
    p_gbuffer(graphicPipeline->GetGBuffer())
  #ifdef BANG_EDITOR
   ,p_selectionFramebuffer(graphicPipeline->GetSelectionFramebuffer())
  #endif
{
}

GraphicPipelinePass::~GraphicPipelinePass()
{
}

