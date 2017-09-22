#include "Bang/UIButton.h"

USING_NAMESPACE_BANG

UIButton::UIButton()
{

}

UIButton::~UIButton()
{

}

UIBorderRect *UIButton::GetBorder() const { return p_border; }
UITextRenderer *UIButton::GetText() const { return p_text; }
UIImageRenderer *UIButton::GetBackground() const { return p_background; }

void UIButton::SetBorder(UIBorderRect *borderRect) { p_border = borderRect; }
void UIButton::SetText(UITextRenderer *textRenderer) { p_text = textRenderer; }
void UIButton::SetBackground(UIImageRenderer *imgRenderer)
{
    p_background = imgRenderer;
}
