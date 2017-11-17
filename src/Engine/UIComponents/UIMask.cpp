#include "Bang/UIMask.h"

#include "Bang/GL.h"
#include "Bang/XMLNode.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"

#include "Bang/Image.h"
#include "Bang/GEngine.h"
#include "Bang/GBuffer.h"

USING_NAMESPACE_BANG

UIMask::UIMask() {}
UIMask::~UIMask() {}

void UIMask::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);
    if  (!m_restoringStencil && renderPass == RenderPass::Canvas)
    {
        PrepareStencilToDrawMask();
    }
}

void UIMask::OnBeforeChildrenRender(RenderPass renderPass)
{
    Component::OnBeforeChildrenRender(renderPass);
    if (renderPass == RenderPass::Canvas) { PrepareStencilToDrawChildren(); }
}

void UIMask::OnAfterChildrenRender(RenderPass renderPass)
{
    Component::OnAfterChildrenRender(renderPass);
    if (renderPass == RenderPass::Canvas) { RestoreStencilBuffer(renderPass); }
}

void UIMask::PrepareStencilToDrawMask()
{
    // Save values for later restoring
    m_maskBefore         = GL::GetColorMask();
    m_stencilFuncBefore  = GL::GetStencilFunc();
    m_stencilOpBefore    = GL::GetStencilOp();

    // Will this mask be drawn?
    GL::SetColorMask(m_drawMask, m_drawMask, m_drawMask, m_drawMask);

    if (IsMasking())
    {
        GL::SetStencilOp( GL::StencilOperation::Incr );
        GL::SetStencilFunc(GL::Function::Equal); // Only increment once
    }
}

void UIMask::PrepareStencilToDrawChildren()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskBefore[0], m_maskBefore[1],
                     m_maskBefore[2], m_maskBefore[3]);

    if (IsMasking())
    {
        // Test and write for current stencil value + 1
        GL::SetStencilValue( GL::GetStencilValue() + 1 );
        GL::SetStencilOp(m_stencilOpBefore);
        GL::SetStencilFunc(GL::Function::Equal); // Mask children
    }
}

void UIMask::RestoreStencilBuffer(RenderPass renderPass)
{
    if (!IsMasking()) { return; }

    // Restore stencil as it was before, decrementing marked mask pixels
    GL::SetColorMask(false, false, false, false);
    GL::SetStencilFunc(GL::Function::Equal);
    GL::SetStencilOp( GL::StencilOperation::Decr );

    m_restoringStencil = true;
    GetGameObject()->Render(renderPass, false);
    m_restoringStencil = false;

    GL::SetStencilValue( GL::GetStencilValue() - 1 );
    GL::SetColorMask(m_maskBefore[0], m_maskBefore[1],
                     m_maskBefore[2], m_maskBefore[3]);
    GL::SetStencilOp(m_stencilOpBefore);
    GL::SetStencilFunc(m_stencilFuncBefore);
}

void UIMask::SetMasking(bool maskEnabled) { m_masking = maskEnabled; }
void UIMask::SetDrawMask(bool drawMask) { m_drawMask = drawMask; }

bool UIMask::IsMasking() const { return m_masking; }
bool UIMask::IsDrawMask() const { return true; } // m_drawMask; }

void UIMask::ImportXML(const XMLNode &xmlInfo)
{
    Component::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Masking"))
    { SetMasking( xmlInfo.Get<bool>("Masking") ); }

    if (xmlInfo.Contains("DrawMask"))
    { SetDrawMask( xmlInfo.Get<bool>("DrawMask") ); }
}

void UIMask::ExportXML(XMLNode *xmlInfo) const
{
    Component::ExportXML(xmlInfo);
    xmlInfo->Set("Masking", IsMasking());
    xmlInfo->Set("DrawMask", IsDrawMask());
}
