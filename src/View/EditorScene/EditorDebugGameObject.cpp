#include "EditorDebugGameObject.h"

EditorDebugGameObject::EditorDebugGameObject() :
    EditorGameObject("EditorDebugGameObject")
{
}

void EditorDebugGameObject::DrawLine(const Vector3 &origin,
                                     const Vector3 &destiny,
                                     float lineWidth,
                                     int msTime,
                                     bool depthTest)
{
    SingleLineRenderer *slr = AddComponent<SingleLineRenderer>();
    slr->SetOrigin(origin);
    slr->SetDestiny(destiny);
    slr->SetLineWidth(lineWidth);

    singleLineRenderers.push_back(slr);
}
