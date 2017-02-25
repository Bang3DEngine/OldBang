#include "GPPass_DepthLayer.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "EditorState.h"

#ifdef BANG_EDITOR
#include "SelectionFramebuffer.h"
#endif

GPPass_DepthLayer::GPPass_DepthLayer(GraphicPipeline *graphicPipeline,
                                     Renderer::DepthLayer depthLayer) :
    GraphicPipelinePass(graphicPipeline),
    m_depthLayer(depthLayer)
{
}

void GPPass_DepthLayer::PrePass(const List<Renderer*> &renderers,
                                const List<GameObject*> &sceneChildren)
{
    GraphicPipelinePass::PrePass(renderers, sceneChildren);

    #ifdef BANG_EDITOR
    if (!p_selectionFramebuffer->IsPassing())
    {
        p_selectionFramebuffer->ClearDepth();
    }
    else
    #endif
    {
        p_gbuffer->ClearAllBuffersExceptColor();
    }
}

bool GPPass_DepthLayer::CanRender(const Renderer *renderer) const
{
    bool dontRenderEditorStuffInGame =
            !(EditorState::IsShowingGameTab() &&
              renderer->gameObject->HasHideFlag(HideFlags::DontSerialize));

    return (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
            renderer->GetDepthLayer() == m_depthLayer &&
            dontRenderEditorStuffInGame;
}

Renderer::DepthLayer GPPass_DepthLayer::GetDepthLayer() const
{
    return m_depthLayer;
}
