#include "EditorSelectionGameObject.h"

EditorSelectionGameObject::EditorSelectionGameObject() :
    EditorGameObject("EditorSelectionGameObject")
{
    AddComponent<Transform>();

    axis = new EditorAxis();
    AddChildWithoutNotifyingHierarchy(axis);
    axis->SetRenderLayer(5);

    bbox = new EditorBBox();
    AddChildWithoutNotifyingHierarchy(bbox);
}
