#include "Bang/GPPass.h"

#include "Bang/Scene.h"
#include "Bang/GBuffer.h"
#include "Bang/SceneManager.h"
#include "Bang/GraphicPipeline.h"

GPPass::GPPass(GraphicPipeline *graphicPipeline, const List<GPPass*> &subPasses)
{
    p_graphicPipeline = graphicPipeline;
    p_gbuffer = graphicPipeline->GetGBuffer();
    #ifdef BANG_EDITOR
    p_selectionFramebuffer = graphicPipeline->GetSelectionFramebuffer();
    #endif

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
    subPass->p_parentPass = this;
    m_subPasses.PushBack(subPass);
}

bool GPPass::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            (renderer && renderer->IsEnabled());
}
