#include "Bang/UIFocusable.h"

#include "Bang/List.h"
#include "Bang/Input.h"
#include "Bang/AARect.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/IFocusListener.h"

USING_NAMESPACE_BANG

UIFocusable::UIFocusable()
{
}

UIFocusable::~UIFocusable()
{
}

bool UIFocusable::CanBeRepeatedInGameObject() const { return false; }

void UIFocusable::PropagateFocusToListeners()
{
    IFocusable::PropagateFocusToListeners();
}

void UIFocusable::PropagateMouseOverToListeners(bool mouseOver)
{
    IFocusable::PropagateMouseOverToListeners(mouseOver);
}


