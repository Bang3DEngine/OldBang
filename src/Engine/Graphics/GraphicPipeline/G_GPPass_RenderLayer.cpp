#include "Bang/G_GPPass_RenderLayer.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"
#include "Bang/EditorState.h"

#ifdef BANG_EDITOR
#include "Bang/SelectionFramebuffer.h"
#endif

G_GPPass_RenderLayer::G_GPPass_RenderLayer(G_GraphicPipeline *graphicPipeline,
                                     Renderer::RenderLayer depthLayer,
                                     const List<G_GPPass*> &subPasses)
    : G_GPPass(graphicPipeline, subPasses)
{
    m_renderLayer = depthLayer;
}

bool G_GPPass_RenderLayer::CanRender(const Renderer *renderer) const
{
    #ifdef BANG_EDITOR
    bool dontRenderEditorStuffInGame =
            !(EditorState::IsShowingGameTab() &&
              renderer->gameObject->GetHideFlags()->IsOn(HideFlag::DontSerialize));
    #else
    bool dontRenderEditorStuffInGame = true;
    #endif

    return G_GPPass::CanRender(renderer) &&
           renderer->GetRenderLayer() == m_renderLayer &&
           dontRenderEditorStuffInGame;
}

Renderer::RenderLayer G_GPPass_RenderLayer::GetRenderLayer() const
{
    return m_renderLayer;
}
