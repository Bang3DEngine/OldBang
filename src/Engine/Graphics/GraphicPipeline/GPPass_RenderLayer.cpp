#include "Bang/GPPass_RenderLayer.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/GBuffer.h"
#include "Bang/EditorState.h"

#ifdef BANG_EDITOR
#include "Bang/SelectionFramebuffer.h"
#endif

GPPass_RenderLayer::GPPass_RenderLayer(GraphicPipeline *graphicPipeline,
                                     Renderer::RenderLayer depthLayer,
                                     const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_renderLayer = depthLayer;
}

bool GPPass_RenderLayer::CanRender(const Renderer *renderer) const
{
    #ifdef BANG_EDITOR
    bool dontRenderEditorStuffInGame =
            !(EditorState::IsShowingGameTab() &&
              renderer->gameObject->GetHideFlags()->IsOn(HideFlag::DontSerialize));
    #else
    bool dontRenderEditorStuffInGame = true;
    #endif

    return GPPass::CanRender(renderer) &&
           renderer->GetRenderLayer() == m_renderLayer &&
           dontRenderEditorStuffInGame;
}

Renderer::RenderLayer GPPass_RenderLayer::GetRenderLayer() const
{
    return m_renderLayer;
}
