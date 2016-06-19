#include "EditorAxisGroup.h"

EditorAxisGroup::EditorAxisGroup()
{
    etag = new EditorTranslateAxisGroup();
    erag = new EditorRotateAxisGroup();

    AddChild(etag);
    AddChild(erag);

    erag->SetEnabled(false);

    AddComponent<Transform>();
}

void EditorAxisGroup::DisableAllGroups()
{
    etag->SetEnabled(false);
    erag->SetEnabled(false);
}

void EditorAxisGroup::OnUpdate()
{
    Toolbar::TransformMode tm = Toolbar::GetInstance()->
                                GetSelectedTransformMode();
    if (tm == Toolbar::TransformMode::Translate)
    {
        DisableAllGroups();
        etag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Rotate)
    {
        DisableAllGroups();
        erag->SetEnabled(true);
    }
    else if (tm == Toolbar::TransformMode::Scale)
    {
        DisableAllGroups();
    }
}


