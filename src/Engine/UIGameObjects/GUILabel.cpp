#include "Bang/GUILabel.h"

#include "Bang/GUIMask.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

GUILabel::GUILabel(const String &content) : UIGameObject("GUILabel")
{
    m_mask = new GUIMask();
    m_mask->SetName("GUILabel_Mask");
    m_mask->AddComponent<UIImageRenderer>(); // Quad mask
    m_mask->SetMasking(true);
    m_mask->SetParent(this);

    m_textContainer = new UIGameObject("GUILabel_TextContainer");
    m_textContainer->SetParent(m_mask);

    p_text = m_textContainer->AddComponent<UITextRenderer>();
    p_text->SetContent(content);
    p_text->SetWrapping(false);
}

GUIMask *GUILabel::GetMask() { return m_mask; }
UITextRenderer *GUILabel::GetText() { return p_text; }
const UITextRenderer *GUILabel::GetText() const { return p_text; }
