#include "UITextInspectorWidget.h"

#include "Bang/Font.h"
#include "Bang/AssetsManager.h"

UITextInspectorWidget::UITextInspectorWidget(UIText *uiText)
    : UIRendererInspectorWidget(uiText)
{
    p_uiText = uiText;
}

UITextInspectorWidget::~UITextInspectorWidget()
{
}

void UITextInspectorWidget::InitExtra()
{
    UIRendererInspectorWidget::InitExtra();

    m_fontAW     = new AttrWidgetFile("Font");
    m_contentAW  = new AttrWidgetString("Content");
    m_textSizeAW = new AttrWidgetFloat("Size");
    m_hSpacingAW = new AttrWidgetFloat("HSpacing");
    m_hAlignAW   = new AttrWidgetEnum<UIText::HorizontalAlignment>(
                            "HAlign",
                            {UIText::HorizontalAlignment::Left,
                             UIText::HorizontalAlignment::Center,
                             UIText::HorizontalAlignment::Right},
                            {"Left", "Center", "Right"}
                        );
    m_vAlignAW   = new AttrWidgetEnum<UIText::VerticalAlignment>(
                            "VAlign",
                            {UIText::VerticalAlignment::Top,
                             UIText::VerticalAlignment::Center,
                             UIText::VerticalAlignment::Bot},
                            {"Top", "Center", "Bot"}
                        );
    m_kerningAW  = new AttrWidgetBool("Kerning");

    InsertAttributeWidget(m_fontAW);
    InsertAttributeWidget(m_contentAW);
    InsertAttributeWidget(m_textSizeAW);
    InsertAttributeWidget(m_hSpacingAW);
    InsertAttributeWidget(m_hAlignAW);
    InsertAttributeWidget(m_vAlignAW);
    InsertAttributeWidget(m_kerningAW);
    m_tintAW->SetLabelText("Color");
}

void UITextInspectorWidget::OnUpdate()
{
    UIRendererInspectorWidget::OnUpdate();

    Font *font = p_uiText->GetFont();
    Path fontPath = font ? font->GetFilepath() : Path::Empty;
    m_fontAW->SetValue(fontPath);

    m_contentAW->SetValue(p_uiText->GetContent());
    m_textSizeAW->SetValue(p_uiText->GetTextSize());
    m_hSpacingAW->SetValue(p_uiText->GetHorizontalSpacing());
    m_hAlignAW->SetValue(p_uiText->GetHorizontalAlignment());
    m_vAlignAW->SetValue(p_uiText->GetVerticalAlignment());
    m_kerningAW->SetValue(p_uiText->GetKerning());
}

void UITextInspectorWidget::OnAttrWidgetValueChanged(
        IAttributeWidget *attrWidget)
{
    UIRendererInspectorWidget::OnAttrWidgetValueChanged(attrWidget);

    if (attrWidget == m_fontAW)
    {
        Font *font = AssetsManager::Load<Font>(m_fontAW->GetPath());
        p_uiText->SetFont(font);
    }
    else if (attrWidget == m_contentAW)
    {
        p_uiText->SetContent(m_contentAW->GetValue());
    }
    else if (attrWidget == m_textSizeAW)
    {
        p_uiText->SetTextSize(m_textSizeAW->GetValue());
    }
    else if (attrWidget == m_hSpacingAW)
    {
        p_uiText->SetHorizontalSpacing(m_hSpacingAW->GetValue());
    }
    else if (attrWidget == m_hAlignAW)
    {
        p_uiText->SetHorizontalAlign(m_hAlignAW->GetValue());
    }
    else if (attrWidget == m_vAlignAW)
    {
        p_uiText->SetVerticalAlign(m_vAlignAW->GetValue());
    }
    else if (attrWidget == m_kerningAW)
    {
        p_uiText->SetKerning(m_kerningAW->GetValue());
    }

    emit Changed(this);
}

void UITextInspectorWidget::OnDestroy()
{
    RendererInspectorWidget::OnDestroy();
}
