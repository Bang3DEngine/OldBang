#include "Bang/RendererFactory.h"

#include "Bang/Renderer.h"
#include "Bang/MaterialFactory.h"

USING_NAMESPACE_BANG

void RendererFactory::ConvertToGizmoRenderer(Renderer *r)
{
    r->SetRenderPass(RenderPass::Gizmos);
    r->SetMaterial( MaterialFactory::GetOnlyColor().Get() );
}
