#include "EditorTranslateAxisGroup.h"

#include "WindowMain.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorTranslateAxisGroup")
{
    axisX = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    axisY = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    axisZ = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    AddChild(axisX);
    AddChild(axisY);
    AddChild(axisZ);

    AddComponent<Transform>();
}

EditorTranslateAxisGroup::~EditorTranslateAxisGroup()
{
}

void EditorTranslateAxisGroup::OnUpdate()
{
}

