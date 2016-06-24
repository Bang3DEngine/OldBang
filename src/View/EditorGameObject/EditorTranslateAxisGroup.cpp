#include "EditorTranslateAxisGroup.h"

#include "WindowMain.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup() : EditorGameObject("EditorTranslateAxisGroup")
{
    axisX = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::X);
    axisY = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Y);
    axisZ = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Z);

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

