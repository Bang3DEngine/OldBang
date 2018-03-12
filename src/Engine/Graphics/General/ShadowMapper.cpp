#include "Bang/ShadowMapper.h"

#include "Bang/AABox.h"
#include "Bang/Scene.h"
#include "Bang/Debug.h"
#include "Bang/Material.h"
#include "Bang/Renderer.h"
#include "Bang/Transform.h"
#include "Bang/DebugRenderer.h"

USING_NAMESPACE_BANG

AABox ShadowMapper::GetSceneCastersAABox(Scene *scene)
{
    Array<Vector3> casterPoints;
    List<Renderer*> renderers = scene->GetComponentsInChildren<Renderer>(true);
    for (Renderer *rend : renderers)
    {
        Material *mat = rend->GetUserMaterial();
        if (mat && mat->GetRenderPass() == RenderPass::Scene)
        {
            Matrix4 localToWorld = rend->GetGameObject()->GetTransform()->GetLocalToWorldMatrix();
            AABox rendAABox = rend->GetAABBox();
            AABox rendAABoxWorld = localToWorld * rendAABox;
            casterPoints.PushBack( rendAABoxWorld.GetPoints() );
        }
    }

    if (casterPoints.IsEmpty()) { return AABox::Empty; }

    AABox sceneAABox;
    sceneAABox.CreateFromPositions(casterPoints);
    // DebugRenderer::RenderAABox(sceneAABox, Color::Green, 0.1f, 1.0f, true);
    return sceneAABox;
}
