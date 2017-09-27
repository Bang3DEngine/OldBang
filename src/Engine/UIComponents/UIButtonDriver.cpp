#include "Bang/UIButtonDriver.h"

USING_NAMESPACE_BANG

UIButtonDriver::UIButtonDriver()
{

}

UIButtonDriver::~UIButtonDriver()
{

}

UIBorderRect *UIButtonDriver::GetBorder() const { return p_border; }
UITextRenderer *UIButtonDriver::GetText() const { return p_text; }
UIImageRenderer *UIButtonDriver::GetBackground() const { return p_background; }
UITintedButton *UIButtonDriver::GetButton() const { return p_button; }

void UIButtonDriver::SetBorder(UIBorderRect *borderRect) { p_border = borderRect; }
void UIButtonDriver::SetText(UITextRenderer *textRenderer) { p_text = textRenderer; }
void UIButtonDriver::SetBackground(UIImageRenderer *imgRenderer)
{
    p_background = imgRenderer;
}
void UIButtonDriver::SetButton(UITintedButton *button)
{
    p_button = button;
}
