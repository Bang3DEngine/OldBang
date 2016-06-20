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
