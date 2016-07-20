#include "EditorTranslateAxisGroup.h"

#include "WindowMain.h"

EditorTranslateAxisGroup::EditorTranslateAxisGroup(GameObject *attachedGameObject)
    : EditorGameObject("EditorTranslateAxisGroup")
{
    p_axisX = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::X, attachedGameObject);
    p_axisY = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Y, attachedGameObject);
    p_axisZ = new EditorTranslateAxis(EditorAxis::EditorAxisDirection::Z, attachedGameObject);

    p_axisX->SetParent(this);
    p_axisY->SetParent(this);
    p_axisZ->SetParent(this);
}

EditorTranslateAxisGroup::~EditorTranslateAxisGroup()
{
}

void EditorTranslateAxisGroup::OnUpdate()
{
}

