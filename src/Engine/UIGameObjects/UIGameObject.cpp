#include "UIGameObject.h"

#include "Bang/Rect.h"
#include "Bang/Input.h"
#include "Bang/RectTransform.h"

UIGameObject::UIGameObject(const String& name) : GameObject(name)
{
    ChangeTransformByRectTransform();
    m_rectTransform = GetComponent<RectTransform>();
}

bool UIGameObject::IsMouseOver() const
{
    Vector2 mouseCoordsNDC = Input::GetMouseCoordsNDC();
    return rectTransform->GetScreenSpaceRectNDC().Contains(mouseCoordsNDC);
}
