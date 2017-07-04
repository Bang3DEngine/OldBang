#include "Bang/G_GPPass.h"

#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"
#include "Bang/SceneManager.h"
#include "Bang/G_GraphicPipeline.h"

G_GPPass::G_GPPass(G_GraphicPipeline *graphicPipeline, const List<G_GPPass*> &subPasses)
{
    p_graphicPipeline = graphicPipeline;
    p_gbuffer = graphicPipeline->GetG_GBuffer();
    #ifdef BANG_EDITOR
    p_selectionG_Framebuffer = graphicPipeline->GetSelectionFramebuffer();
    #endif

    for (G_GPPass *subPass : subPasses)
    {
        m_subPasses.PushBack(subPass);
        subPass->p_parentPass = this;
    }
}

G_GPPass::~G_GPPass()
{
    for (G_GPPass *subPass : m_subPasses)
    {
        delete subPass;
    }
}

void G_GPPass::PrePass(const List<Renderer*> &renderers,
                     const List<GameObject*> &sceneChildren)
{
}
void G_GPPass::InPass(const List<Renderer*> &renderers,
                    const List<GameObject*> &sceneChildren)
{
}
void G_GPPass::PostPass(const List<Renderer*> &renderers,
                      const List<GameObject*> &sceneChildren)
{
}

void G_GPPass::Pass(const List<Renderer*> &renderers,
                  const List<GameObject*> &sceneChildren)
{
    p_scene = SceneManager::GetActiveScene();

    PrePass(renderers, sceneChildren);
    InPass(renderers, sceneChildren);

    for (G_GPPass *gpSubPass : m_subPasses)
    {
        gpSubPass->Pass(renderers, sceneChildren);
    }

    PostPass(renderers, sceneChildren);
}

void G_GPPass::AddSubPass(G_GPPass *subPass)
{
    subPass->p_parentPass = this;
    m_subPasses.PushBack(subPass);
}

bool G_GPPass::CanRender(const Renderer *renderer) const
{
    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            (renderer && renderer->IsEnabled());
}
