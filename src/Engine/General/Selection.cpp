#include "Bang/Selection.h"

#include "Bang/Input.h"
#include "Bang/Screen.h"
#include "Bang/GraphicPipeline.h"
#include "Bang/SelectionFramebuffer.h"

GameObject *Selection::GetOveredGameObject()
{
    if (!Input::IsMouseInsideScreen()) { return nullptr; }

    Vector2i coords = Input::GetMouseCoords();
    SelectionFramebuffer *sfb = Selection::GetSelectionFramebuffer();
    return sfb->GetGameObjectInPosition(coords);
}

SelectionFramebuffer *Selection::GetSelectionFramebuffer()
{
    return GraphicPipeline::GetActive()->GetSelectionFramebuffer();
}
