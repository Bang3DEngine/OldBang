#include "Bang/UIFocusable.h"

#include "Bang/Rect.h"
#include "Bang/List.h"
#include "Bang/Input.h"
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

    if (HasFocus())
    {
        EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnFocusTaken, this);
        GetGameObject()->Propagate(&IFocusListener::OnFocusTaken,
                              GetGameObject()->GetComponents<IFocusListener>(),
                              this);
    }
    else
    {
        EventEmitter<IFocusListener>::
                PropagateToListeners(&IFocusListener::OnFocusLost, this);
        GetGameObject()->Propagate(&IFocusListener::OnFocusLost,
                              GetGameObject()->GetComponents<IFocusListener>(),
                              this);
    }
}

void UIFocusable::PropagateMouseOverToListeners(bool mouseOver)
{
    IFocusable::PropagateMouseOverToListeners(mouseOver);
}


