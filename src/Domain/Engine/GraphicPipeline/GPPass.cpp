#include "GPPass.h"

#include "Scene.h"
#include "GBuffer.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"

GPPass::GPPass(GraphicPipeline *graphicPipeline, const List<GPPass*> &subPasses)
{
    p_graphicPipeline = graphicPipeline;
    p_gbuffer = graphicPipeline->GetGBuffer();
    p_selectionFramebuffer = graphicPipeline->GetSelectionFramebuffer();
    for (GPPass *subPass : subPasses)
    {
        m_subPasses.PushBack(subPass);
        subPass->p_parentPass = this;
    }
}

GPPass::~GPPass()
{
    for (GPPass *subPass : m_subPasses)
    {
        delete subPass;
    }
}

void GPPass::PrePass(const List<Renderer*> &renderers,
                                  const List<GameObject*> &sceneChildren)
{
}
void GPPass::InPass(const List<Renderer*> &renderers,
                                 const List<GameObject*> &sceneChildren)
{
}
void GPPass::PostPass(const List<Renderer*> &renderers,
                                   const List<GameObject*> &sceneChildren)
{
}

void GPPass::Pass(const List<Renderer*> &renderers,
                               const List<GameObject*> &sceneChildren)
{
    p_scene = SceneManager::GetActiveScene();

    PrePass(renderers, sceneChildren);
    InPass(renderers, sceneChildren);

    for (GPPass *gpSubPass : m_subPasses)
    {
        gpSubPass->Pass(renderers, sceneChildren);
    }

    PostPass(renderers, sceneChildren);
}

void GPPass::AddSubPass(GPPass *subPass)
{
    m_subPasses.PushBack(subPass);
    subPass->p_parentPass = this;
}

bool GPPass::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            CAN_USE_COMPONENT(renderer);
}
