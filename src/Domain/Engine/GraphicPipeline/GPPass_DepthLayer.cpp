#include "Bang/GPPass_DepthLayer.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/GBuffer.h"
#include "Bang/EditorState.h"

#ifdef BANG_EDITOR
#include "Bang/SelectionFramebuffer.h"
#endif

GPPass_DepthLayer::GPPass_DepthLayer(GraphicPipeline *graphicPipeline,
                                     Renderer::DepthLayer depthLayer,
                                     const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
    m_depthLayer = depthLayer;
}

void GPPass_DepthLayer::PrePass(const List<Renderer*> &renderers,
                                const List<GameObject*> &sceneChildren)
{
    GPPass::PrePass(renderers, sceneChildren);

    #ifdef BANG_EDITOR
    if (p_selectionFramebuffer->IsPassing())
    {
        // p_selectionFramebuffer->ClearDepth();
    }
    else
    #endif
    {
        // p_gbuffer->ClearDepth();
    }
}

bool GPPass_DepthLayer::CanRender(const Renderer *renderer) const
{
    #ifdef BANG_EDITOR
    bool dontRenderEditorStuffInGame =
            !(EditorState::IsShowingGameTab() &&
              renderer->gameObject->GetHideFlags()->IsOn(HideFlag::DontSerialize));
    #else
    bool dontRenderEditorStuffInGame = true;
    #endif

    return GPPass::CanRender(renderer) &&
           renderer->GetDepthLayer() == m_depthLayer &&
           dontRenderEditorStuffInGame;
}

Renderer::DepthLayer GPPass_DepthLayer::GetDepthLayer() const
{
    return m_depthLayer;
}
