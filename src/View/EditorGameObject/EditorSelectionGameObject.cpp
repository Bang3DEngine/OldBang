#include "EditorSelectionGameObject.h"

EditorSelectionGameObject::EditorSelectionGameObject() :
    EditorGameObject("EditorSelectionGameObject")
{
    AddComponent<Transform>();

    axis = new EditorAxis();
    AddChildWithoutNotifyingHierarchy(axis);

    bbox = new EditorBBox();
    AddChildWithoutNotifyingHierarchy(bbox);
}
