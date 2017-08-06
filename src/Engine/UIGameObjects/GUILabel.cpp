#include "Bang/GUILabel.h"

#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"

GUILabel::GUILabel(const String &content) : UIGameObject("GUILabel")
{
    p_text = AddComponent<UITextRenderer>();
    p_text->SetContent(content);
    p_text->SetHorizontalWrapMode(WrapMode::Hide);
    p_text->SetVerticalWrapMode(WrapMode::Overflow);
    rectTransform->SetMargins(5);
}

UITextRenderer *GUILabel::GetText() { return p_text; }
const UITextRenderer *GUILabel::GetText() const { return p_text; }
