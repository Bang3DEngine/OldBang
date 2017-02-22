#include "GPPass_Selection.h"

#include "Debug.h"
#include "Scene.h"
#include "GBuffer.h"
#include "Material.h"
#include "Renderer.h"
#include "GraphicPipeline.h"
#include "SelectionFramebuffer.h"

GPPass_Selection::GPPass_Selection(GraphicPipeline *graphicPipeline)
    : GraphicPipelinePass(graphicPipeline)
{
}

void GPPass_Selection::InPass(const List<Renderer *> &renderers,
                                const List<GameObject *> &sceneChildren)
{
    GraphicPipelinePass::InPass(renderers, sceneChildren);

    p_gbuffer->SetAllDrawBuffers();
    for (Renderer *rend : renderers)
    {
        if (!CanRender(rend)) { continue; }
        p_selectionFramebuffer->RenderForSelectionBuffer(rend);
    }
}

bool GPPass_Selection::CanRender(const Renderer *renderer) const
{
    return renderer &&
           (p_parentPass ? p_parentPass->CanRender(renderer) : true) &&
           !renderer->gameObject->HasHideFlag(HideFlags::HideInSelection);
}

