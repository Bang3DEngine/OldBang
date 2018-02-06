#include "Bang/UIRectMask.h"

#include "Bang/GL.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

UIRectMask::UIRectMask() {}
UIRectMask::~UIRectMask() {}

void UIRectMask::OnBeforeChildrenRender(RenderPass renderPass)
{
    Component::OnBeforeChildrenRender(renderPass);

    if (IsMasking() && renderPass == RenderPass::Canvas)
    {
        m_wasScissorEnabled = GL::IsEnabled(GL::Test::Scissor);
        m_prevScissor = GL::GetScissorRect();

        AARecti rectPx( GetGameObject()->GetRectTransform()->GetViewportRect() );

        GL::Enable(GL::Test::Scissor);
        GL::ScissorIntersecting(rectPx);
    }
}

void UIRectMask::OnAfterChildrenRender(RenderPass renderPass)
{
    Component::OnAfterChildrenRender(renderPass);

    if (IsMasking() && renderPass == RenderPass::Canvas)
    {
        // Restore
        GL::Scissor(m_prevScissor);
        GL::SetEnabled(GL::Test::Scissor, m_wasScissorEnabled);
    }
}

void UIRectMask::SetMasking(bool maskEnabled) { m_masking = maskEnabled; }
bool UIRectMask::IsMasking() const { return m_masking; }

void UIRectMask::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Masking"))
    { SetMasking( xmlInfo.Get<bool>("Masking") ); }
}

void UIRectMask::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);

    xmlInfo->Set("Masking", IsMasking());
}
