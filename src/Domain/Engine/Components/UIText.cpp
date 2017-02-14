#include "UIText.h"

#include "Debug.h"

#include "Font.h"
#include "Debug.h"
#include "Scene.h"
#include "Screen.h"
#include "Camera.h"
#include "XMLNode.h"
#include "Material.h"
#include "Transform.h"
#include "UIGameObject.h"
#include "SceneManager.h"
#include "RectTransform.h"
#include "AssetsManager.h"
#include "GraphicPipeline.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "SelectionFramebuffer.h"
#endif

UIText::UIText() : UIRenderer()
{
    m_material = AssetsManager::Load<Material>("Materials/UI/D2G_UIText.bmat",
                                               true);
    m_font = AssetsManager::Load<Font>("Fonts/UbuntuFont.bfont", true);
}

UIText::~UIText()
{
}

void UIText::Render() const
{
    glDepthFunc(GL_LEQUAL);
    RenderText();
}

void UIText::RenderForSelectionFramebufferWithoutBindingMaterial() const
{
    RenderWithoutBindingMaterial();
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

void UIText::SetHorizontalSpacing(int horizontalSpacing)
{
    m_horizontalSpacing = horizontalSpacing;
}

int UIText::GetHorizontalSpacing() const
{
    return m_horizontalSpacing;
}

Rect UIText::GetBoundingRect(Camera *camera) const
{
    return Rect::ScreenRect;
}

void UIText::RenderText() const
{
    ASSERT(m_font);
    ASSERT(m_material);
    ASSERT(m_material->GetShaderProgram());
    m_material->SetDiffuseColor(m_textColor);

    RectTransform *rtrans = gameObject->GetComponent<RectTransform>();
    ASSERT(rtrans);

    UIGameObject *copyGo = new UIGameObject();
    RectTransform *rtransCopy = copyGo->GetComponent<RectTransform>();
    rtrans->CloneInto(rtransCopy);

    constexpr float scaleFactor = 0.05f;
    Rect screenRectNDC = rtrans->GetScreenSpaceRect(true);
    Vector2 textSizeNDC = m_textSize / Screen::GetSize() * scaleFactor;
    float horizontalSpacingNDC = m_horizontalSpacing * textSizeNDC.x;

    GameObject *originalParent = gameObject->parent;
    gameObject->SetParent(nullptr);

    float xDisplacementFromBeginning = 0.0f;
    rtrans->SetMargins(0,0,0,0);
    for (int i = 0; i < m_content.Length(); ++i)
    {
        char c = m_content[i];
        const Font::CharGlyphMetrics &charMetrics = m_font->GetCharacterMetrics(c);
        Texture2D *charTexture = m_font->GetCharacterTexture(c);

        Vector2 charSizePx (charMetrics.width, charMetrics.height);
        if (!charTexture) { charSizePx = Vector2(40,40); }

        charSizePx *= textSizeNDC;

        Vector2 charAnchorMin = screenRectNDC.GetMin();
        charAnchorMin.x += xDisplacementFromBeginning;

        // Apply Bearings(X/Y)
        Vector2 bearing (charMetrics.bearingX, charMetrics.bearingY);
        charAnchorMin   += bearing * textSizeNDC;
        charAnchorMin.y -= (charMetrics.height - charMetrics.originY) *
                            textSizeNDC.y;

        // Set corresponding char texture to material
        rtrans->SetAnchorMin(charAnchorMin);
        rtrans->SetAnchorMax(charAnchorMin + charSizePx);
        if (charTexture)
        {
            m_material->SetTexture(charTexture);
            UIRenderer::Render();
            m_material->SetTexture(nullptr);
        }

        // Move to the right the advance distance
        float advance = charMetrics.advance;
        if (i > 0)
        {
            // Try to get the kerningX instead of advance
            float advx = m_font->GetKerningX(m_content[i-1], m_content[i]);
            if (advx > 0) { advance = advx; }
        }
        xDisplacementFromBeginning += advance * textSizeNDC.x;
        //xDisplacementFromBeginning += horizontalSpacingNDC;
    }

    rtransCopy->CloneInto(rtrans);
    delete copyGo;
    gameObject->SetParent(originalParent);
}

void UIText::ReadXMLInfo(const XMLNode *xmlInfo)
{
    UIRenderer::ReadXMLInfo(xmlInfo);

    String fontFilepath = xmlInfo->GetFilepath("Font");
    if (!fontFilepath.Empty())
    {
        m_font = AssetsManager::Load<Font>(fontFilepath);
    }

    m_textColor = xmlInfo->GetColor("Color");
    SetContent(xmlInfo->GetString("Content"));
    SetTextSize(xmlInfo->GetFloat("TextSize"));
    SetHorizontalSpacing(xmlInfo->GetFloat("HorizontalSpacing"));
}

void UIText::FillXMLInfo(XMLNode *xmlInfo) const
{
    UIRenderer::FillXMLInfo(xmlInfo);
    xmlInfo->SetTagName(GetName());

    xmlInfo->SetFilepath("Font", m_font ? m_font->GetFilepath() : "", "bfont");
    xmlInfo->SetColor("Color", m_textColor);
    xmlInfo->SetString("Content", m_content, {XMLProperty::Inline});
    xmlInfo->SetFloat("TextSize", m_textSize);
    xmlInfo->SetFloat("HorizontalSpacing", m_horizontalSpacing);

    xmlInfo->GetAttribute("Tint")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Mesh")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("Material")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("LineWidth")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("IsTransparent")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("DrawWireframe")->SetProperty({XMLProperty::Hidden});
    xmlInfo->GetAttribute("ReceivesLighting")->SetProperty({XMLProperty::Hidden});
}
