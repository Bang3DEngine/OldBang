#include "Bang/GPPass_Selection.h"

#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/G_GBuffer.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/GPPass_RenderLayer.h"
#include "Bang/SelectionFramebuffer.h"

GPPass_Selection::GPPass_Selection(GraphicPipeline *graphicPipeline,
                                   const List<GPPass*> &subPasses)
    : GPPass(graphicPipeline, subPasses)
{
}

void GPPass_Selection::InPass(const List<Renderer *> &renderers,
                              const List<GameObject *> &sceneChildren)
{
    GPPass::InPass(renderers, sceneChildren);

    p_selectionFramebuffer->ClearDepth();
    p_selectionFramebuffer->SetAllDrawBuffers();

    GPPass_RenderLayer *parentPassRenderLayer =
            Object::SCast<GPPass_RenderLayer>(p_parentPass);

    if (parentPassRenderLayer->GetRenderLayer() != Renderer::Gizmos)
    {
        for (Renderer *rend : renderers)
        {
            if (!CanRender(rend)) { continue; }
            p_selectionFramebuffer->RenderForSelectionBuffer(rend);
        }
    }
    else
    {
        GL::SetTestDepth(true);
        for (GameObject *go : sceneChildren)
        {
            go->_OnDrawGizmos(true, false);
        }

        GL::SetTestDepth(false);
        for (GameObject *go : sceneChildren)
        {
            go->_OnDrawGizmos(false, false);
        }

        for (GameObject *go : sceneChildren)
        {
            go->_OnDrawGizmos(false, true);
        }
        GL::SetTestDepth(true);
    }
}

bool GPPass_Selection::CanRender(const Renderer *renderer) const
{
    return GPPass::CanRender(renderer) &&
           !renderer->gameObject->GetHideFlags()->IsOn(HideFlag::HideInSelection);
}

