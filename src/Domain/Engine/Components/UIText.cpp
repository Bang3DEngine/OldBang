#include "UIText.h"

#include "Font.h"

UIText::UIText() : UIRenderer()
{
    m_material = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/D2G_UIText.bmat");
    m_font = AssetsManager::LoadAsset<Font>("Assets/Engine/Fonts/PrettyFont.bfont");
}

UIText::~UIText()
{
    delete m_material;
}

void UIText::Render() const
{
    UIRenderer::Render();

    if (m_font)
    {
        if (m_material)
        {
            m_material->SetTexture(m_font->GetFontSheet());
        }
    }
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
}

ICloneable *UIText::Clone() const
{
    return UIRenderer::Clone();
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
    m_content = xmlInfo->GetString("Content");
}

void UIText::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);

    xmlInfo->SetTagName(GetName());

    xmlInfo->SetFilepath("Font", m_font ? m_font->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", m_textColor);
    xmlInfo->SetString("Content", "Text content", {XMLProperty::Inline});

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
    // Nothing
}
