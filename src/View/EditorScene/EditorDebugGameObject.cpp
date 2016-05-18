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
    LineRenderer *lr = AddComponent<LineRenderer>();
    lr->SetOrigin(origin);
    lr->SetDestiny(destiny);
    lr->SetLineWidth(lineWidth);

    lineRenderers.push_back(lr);
}
