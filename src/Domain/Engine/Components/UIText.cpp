#include "UIText.h"

#include "Font.h"

UIText::UIText() : UIRenderer()
{
    m_material = nullptr;
    m_material = m_letterQuadMaterial = AssetsManager::LoadAsset<Material>("Assets/Engine/Materials/UI/D2G_UIText.bmat");
    m_font = AssetsManager::LoadAsset<Font>("Assets/Engine/Fonts/GreatFont.bfont");
}

UIText::~UIText()
{
    delete m_material;
}

void UIText::Render() const
{
    if (m_font)
    {
        Vector3 initialScale = transform->GetScale();
        Vector3 initialPosition = transform->GetPosition();

        float characterStep = m_textSize * 1.0f / 100.0f;
        Vector3 scale = Vector3(characterStep, characterStep, 1.0f) * transform->GetScale();
        transform->SetScale(scale);
        for (int i = 0; i < m_content.Length(); ++i)
        {
            char c = m_content[i];
            if (c != ' ')
            {
                Texture2D *charTexture = m_font->GetCharacterTexture(c);

                // Set corresponding char texture to material and render
                m_material->SetTexture(charTexture);
                UIRenderer::Render();
            }

            // Move to the right a char space
            transform->Translate(Vector3::Right * characterStep);
        }
        transform->SetPosition(initialPosition);
        transform->SetScale(initialScale);
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

void UIText::SetContent(const String &content)
{
    m_content = content;

    /*
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        MeshRenderer *letterQuad = new MeshRenderer();
        letterQuad->SetMaterial(m_letterQuadMaterial);
        m_letterQuads.Add(letterQuad);
    }
    */
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

void UIText::DeleteCurrentLetterQuads()
{
    /*
    for (MeshRenderer *meshRend : m_letterQuads)
    {
        gameObject->RemoveComponent(meshRend);
        delete meshRend;
    }
    m_letterQuads.Clear();
    */
}
