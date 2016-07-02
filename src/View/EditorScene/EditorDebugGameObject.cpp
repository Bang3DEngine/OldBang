#include "EditorDebugGameObject.h"

EditorDebugGameObject::EditorDebugGameObject() :
    EditorGameObject("EditorDebugGameObject")
{
}

EditorDebugGameObject::~EditorDebugGameObject()
{
}

void EditorDebugGameObject::DrawLine(const Vector3 &origin,
                                     const Vector3 &destiny,
                                     const Vector3 &color,
                                     float lineWidth,
                                     float livingTimeSecs,
                                     bool depthTest)
{
    SingleLineRenderer *slr = AddComponent<SingleLineRenderer>();
    slr->SetOrigin(origin);
    slr->SetDestiny(destiny);
    slr->SetLineWidth(lineWidth);

    debugLines.push_back(DebugLine(slr, livingTimeSecs));
}

void EditorDebugGameObject::DrawLineScreen(const glm::vec2 &origin, const glm::vec2 &destiny,
                                           const Vector3 &color, float lineWidth,
                                           float livingTimeSecs, bool depthTest)
{
    SingleLineRenderer *slr = AddComponent<SingleLineRenderer>();
    slr->SetIgnoreModelMatrix(true);
    slr->SetIgnoreViewMatrix(true);
    slr->SetIgnoreProjectionMatrix(true);
    slr->SetOrigin(Vector3(origin.x, origin.y, 0.0f));
    slr->SetDestiny(Vector3(destiny.x, destiny.y, 0.0f));
    slr->SetLineWidth(lineWidth);

    debugLines.push_back(DebugLine(slr, livingTimeSecs));
}

void EditorDebugGameObject::OnUpdate()
{
    float dTime = Time::GetDeltaTime();
    for (auto it = debugLines.begin(); it != debugLines.end(); ++it)
    {
        DebugLine &dl = *it;
        dl.elapsedTimeSecs += dTime;
        if(dl.elapsedTimeSecs >= dl.livingTimeSecs)
        {
            RemoveComponent(dl.slr);
            it = debugLines.erase(it);
        }
    }
}
