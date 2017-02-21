#include "GraphicPipelinePass.h"

#include "Scene.h"
#include "GBuffer.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"

void GraphicPipelinePass::PrePass(const List<Renderer*> &renderers)
{
}
void GraphicPipelinePass::InPass(const List<Renderer*> &renderers)
{
}
void GraphicPipelinePass::PostPass(const List<Renderer*> &renderers)
{
}

void GraphicPipelinePass::Pass(const List<Renderer*> &renderers)
{
    p_scene = SceneManager::GetActiveScene();

    PrePass(renderers);
    InPass(renderers);

    for (GraphicPipelinePass *gpSubPass : m_subPasses)
    {
        gpSubPass->Pass(renderers);
    }

    PostPass(renderers);
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
{
}

GraphicPipelinePass::~GraphicPipelinePass()
{
}

