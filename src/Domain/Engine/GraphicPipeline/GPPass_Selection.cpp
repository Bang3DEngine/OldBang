#include "GPPass_Selection.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"
#include "GPPass_DepthLayer.h"
#include "SelectionFramebuffer.h"

GPPass_Selection::GPPass_Selection(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_Selection::InPass(const List<Renderer *> &renderers,
                              const List<GameObject *> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    p_selectionFramebuffer->SetAllDrawBuffers();

    GPPass_DepthLayer *parentDepthLayer =
            Object::SCast<GPPass_DepthLayer>(p_parentPass);

    if (parentDepthLayer->GetDepthLayer() != Renderer::DepthLayerGizmos)
    {
        for (Renderer *rend : renderers)
        {
            if (!CanRender(rend)) { continue; }
            p_selectionFramebuffer->RenderForSelectionBuffer(rend);
        }
    }
    else
    {
        for (GameObject *go : sceneChildren)
        {
            go->_OnDrawGizmos();
        }

        p_selectionFramebuffer->ClearDepth();

        for (GameObject *go : sceneChildren)
        {
            go->_OnDrawGizmosOverlay();
        }
    }
}

bool GPPass_Selection::CanRender(const Renderer *renderer) const
{
    return renderer &&
           (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
           !renderer->gameObject->HasHideFlag(HideFlags::HideInSelection);
}

