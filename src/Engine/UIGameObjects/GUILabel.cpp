#include "Bang/GUILabel.h"

#include "Bang/UIText.h"
#include "Bang/RectTransform.h"

GUILabel::GUILabel(const String &content) : UIGameObject()
{
    p_text = AddComponent<UIText>();
    p_text->SetContent(content);
    p_text->SetHorizontalWrapMode(WrapMode::Hide);
    p_text->SetVerticalWrapMode(WrapMode::Overflow);
    rectTransform->SetMargins(5);
}

UIText *GUILabel::GetText() { return p_text; }
const UIText *GUILabel::GetText() const { return p_text; }
