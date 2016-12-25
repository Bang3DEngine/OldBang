#include "UIText.h"

#include "Font.h"

UIText::UIText() : UIRenderer()
{
    m_material = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/D2G_UIText.bmat");
    m_font = AssetsManager::LoadAsset<Font>("Assets/Engine/Fonts/GreatFont.bfont");
}

UIText::~UIText()
{
}

void UIText::Render() const
{
    NONULL(m_font); NONULL(m_material); NONULL(m_material->GetShaderProgram());

    m_material->SetDiffuseColor(m_textColor);

    Vector3 initialScale = transform->GetScale();
    Vector3 initialPosition = transform->GetPosition();

    float scaleFactor =  ( m_textSize * (1.0f / 8000.0f) );
    Vector3 charScaleFactor = transform->GetScale() * scaleFactor;
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        const Font::CharGlyphMetrics &charMetrics = m_font->GetCharacterMetrics(c);
        Texture2D *charTexture = m_font->GetCharacterTexture(c);

        Vector3 quadScale = charScaleFactor * Vector3(charMetrics.width, charMetrics.height, 1.0f) * 1.5f;
        transform->SetScale(quadScale);          // The quad must have the dimensions of the char
        transform->Translate(-quadScale / 2.0f); // Move from center to topleft

        // Apply Bearings(X/Y)
        Vector3 bearing = Vector3(charMetrics.bearingX, charMetrics.bearingY, 0) * charScaleFactor;
        transform->Translate(bearing);

        m_material->SetTexture(charTexture); // Set corresponding char texture to material
        UIRenderer::Render(); // RENDER THE CHAR !!!

        // Unapply Bearings(X/Y)
        transform->Translate(-bearing);
        transform->Translate(quadScale / 2.0f); // Move from topleft to center again

        // Move to the right the advance distance
        float advance = charMetrics.advance;
        if (i > 0)
        {
            float advx = m_font->GetKerningX(m_content[i-1], m_content[i]);
            if (advx > 0) { advance = advx; } // Try to get the kerningX instead of advance
        }
        advance *= charScaleFactor.x;
        transform->Translate(Vector3::Right * advance);
    }
    transform->SetPosition(initialPosition);
    transform->SetScale(initialScale);
}

const String UIText::ToString() const
{
    return "UIText";
}

String UIText::GetName() const
{
    return "UIText";
}

void UIText::CloneInto(ICloneable *clone) const
{
    UIRenderer::CloneInto(clone);
    UIText *text = static_cast<UIText*>(clone);
    text->SetContent(m_content);
    text->SetTextSize(m_textSize);
    text->m_font = m_font;
    text->m_textColor = m_textColor;
}

ICloneable *UIText::Clone() const
{
    UIText *text = new UIText();
    CloneInto(text);
    return text;
}

void UIText::OnInspectorXMLNeeded(XMLNode *xmlInfo) const
{
    FillXMLInfo(xmlInfo);
}

void UIText::OnInspectorXMLChanged(const XMLNode *xmlInfo)
{
    ReadXMLInfo(xmlInfo);
}

void UIText::ReadXMLInfo(const XMLNode *xmlInfo)
{
    UIRenderer::ReadXMLInfo(xmlInfo);

    String fontFilepath = xmlInfo->GetFilepath("Font");
    if (!fontFilepath.Empty())
    {
        m_font = AssetsManager::LoadAsset<Font>(fontFilepath);
    }

    m_textColor = xmlInfo->GetColor("Color");
    SetContent(xmlInfo->GetString("Content"));
    SetTextSize(xmlInfo->GetFloat("TextSize"));
}

void UIText::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    xmlInfo->SetFilepath("Font", m_font ? m_font->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", m_textColor);
    xmlInfo->SetString("Content", m_content, {XMLProperty::Inline});
    xmlInfo->SetFloat("TextSize", m_textSize);

    xmlInfo->GetAttribute("Tint")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("ReceivesLighting")->SetProperty({XMLProperty::Hidden});
}

void UIText::RenderCustomPR() const
{
    UIRenderer::RenderCustomPR();
}

void UIText::SetContent(const String &content)
{
    m_content = content;
}

const String &UIText::GetContent() const
{
    return m_content;
}

void UIText::SetTextSize(int size)
{
    m_textSize = size;
}

int UIText::GetTextSize() const
{
    return m_textSize;
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}
