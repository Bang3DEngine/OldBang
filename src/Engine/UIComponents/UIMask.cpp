#include "Bang/UIMask.h"

#include "Bang/GL.h"
#include "Bang/XMLNode.h"
#include "Bang/UIRenderer.h"
#include "Bang/UIGameObject.h"

UIMask::UIMask() {}
UIMask::~UIMask() {}

void UIMask::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);
    if  (!m_restoringStencil &&
         renderPass == RenderPass::Canvas)
    {
        PrepareStencilToDrawMask();
    }
}

void UIMask::OnBeforeChildrenRender(RenderPass renderPass)
{
    Component::OnBeforeChildrenRender(renderPass);
    if (renderPass == RenderPass::Canvas)
    {
        PrepareStencilToDrawChildren();
    }
}

void UIMask::OnChildrenRendered(RenderPass renderPass)
{
    Component::OnChildrenRendered(renderPass);
    if (renderPass == RenderPass::Canvas)
    {
        RestoreStencilBuffer(renderPass);
    }
}

void UIMask::PrepareStencilToDrawMask()
{
    // Save values for later restoring
    m_maskRBefore = GL::IsColorMaskR();
    m_maskGBefore = GL::IsColorMaskG();
    m_maskBBefore = GL::IsColorMaskB();
    m_maskABefore = GL::IsColorMaskA();
    m_stencilTestBefore  = GL::IsStencilTest();
    m_stencilWriteBefore = GL::IsStencilWrite();
    m_stencilOpBefore    = GL::GetStencilOp();

    // Will this mask be drawn?
    GL::SetColorMask(m_drawMask, m_drawMask, m_drawMask, m_drawMask);

    if (IsMasking())
    {
        GL::SetStencilOp( GL_INCR );
        GL::SetStencilTest(true);  // Only increment once
        GL::SetStencilWrite(true); // This rendering will write to the stencil
    }
}

void UIMask::PrepareStencilToDrawChildren()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskRBefore, m_maskGBefore, m_maskBBefore, m_maskABefore);

    if (IsMasking())
    {
        // Test and write for current stencil value + 1
        GL::SetStencilValue( GL::GetStencilValue() + 1 );
        GL::SetStencilOp(m_stencilOpBefore);
        GL::SetStencilWrite(m_stencilWriteBefore); // Restore stencil write
        GL::SetStencilTest(true); // Mask children
    }
}

void UIMask::RestoreStencilBuffer(RenderPass renderPass)
{
    if (!IsMasking()) { return; }

    // Restore stencil as it was before, decrementing marked mask pixels
    GL::SetColorMask(false, false, false, false);
    GL::SetStencilTest(true);
    GL::SetStencilWrite(true);
    GL::SetStencilOp(GL_DECR);

    m_restoringStencil = true;
    GetGameObject()->Render(renderPass, false);
    m_restoringStencil = false;

    GL::SetStencilValue( GL::GetStencilValue() - 1 );
    GL::SetColorMask(m_maskRBefore, m_maskGBefore,
                     m_maskBBefore, m_maskABefore);
    GL::SetStencilOp(m_stencilOpBefore);
    GL::SetStencilWrite(m_stencilWriteBefore);
    GL::SetStencilTest(m_stencilTestBefore);
}

void UIMask::SetMasking(bool maskEnabled) { m_masking = maskEnabled; }
void UIMask::SetDrawMask(bool drawMask) { m_drawMask = drawMask; }

bool UIMask::IsMasking() const { return m_masking; }
bool UIMask::IsDrawMask() const { return m_drawMask; }

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
