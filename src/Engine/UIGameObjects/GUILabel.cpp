#include "Bang/GUILabel.h"

#include "Bang/GUIScrollArea.h"
#include "Bang/RectTransform.h"

GUILabel::GUILabel(const String &content) : UIGameObject("GUILabel")
{
    m_scrollArea = new GUIScrollArea();
    m_scrollArea->SetName("GUILabel_ScrollArea");
    m_scrollArea->SetParent(this);

    m_textContainer = new UIGameObject("GUILabel_TextContainer");
    m_scrollArea->AddChild(m_textContainer);
    m_scrollArea->SetMasking(false);

    p_text = m_textContainer->AddComponent<UITextRenderer>();
    p_text->SetContent(content);
    p_text->SetWrapping(false);
    m_textContainer->rectTransform->SetMargins(5);
}

GUIScrollArea *GUILabel::GetScrollArea() { return m_scrollArea; }
UITextRenderer *GUILabel::GetText() { return p_text; }
const UITextRenderer *GUILabel::GetText() const { return p_text; }
