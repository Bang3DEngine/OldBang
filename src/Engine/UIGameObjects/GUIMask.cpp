#include "Bang/GUIMask.h"

#include "Bang/GL.h"
#include "Bang/UIRenderer.h"
#include "Bang/UIGameObject.h"


#include "Bang/Paths.h"
#include "Bang/G_GBuffer.h"
#include "Bang/GraphicPipeline.h"


GUIMask::GUIMask() {}
GUIMask::~GUIMask() {}

void GUIMask::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);
    if  (!m_restoringStencil &&
         renderPass == RenderPass::Canvas)
    {
        PrepareStencilToDrawMask();
    }
}

void GUIMask::OnBeforeChildrenRender(RenderPass renderPass)
{
    Component::OnBeforeChildrenRender(renderPass);
    if (renderPass == RenderPass::Canvas)
    {
        PrepareStencilToDrawChildren();
    }
}

void GUIMask::OnChildrenRendered(RenderPass renderPass)
{
    Component::OnChildrenRendered(renderPass);
    if (renderPass == RenderPass::Canvas)
    {
        RestoreStencilBuffer(renderPass);
    }
}

void GUIMask::PrepareStencilToDrawMask()
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

    if (GetMasking())
    {
        GL::SetStencilOp( GL_INCR );
        GL::SetStencilTest(true);  // Only increment once
        GL::SetStencilWrite(true); // This rendering will write to the stencil
    }
}

void GUIMask::PrepareStencilToDrawChildren()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskRBefore, m_maskGBefore, m_maskBBefore, m_maskABefore);

    if (GetMasking())
    {
        // Test and write for current stencil value + 1
        GL::SetStencilValue( GL::GetStencilValue() + 1 );
        GL::SetStencilOp(m_stencilOpBefore);
        GL::SetStencilWrite(m_stencilWriteBefore); // Restore stencil write
        GL::SetStencilTest(true); // Mask children
    }
}

void GUIMask::RestoreStencilBuffer(RenderPass renderPass)
{
    if (!GetMasking()) { return; }

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

void GUIMask::SetMasking(bool maskEnabled) { m_masking = maskEnabled; }
void GUIMask::SetDrawMask(bool drawMask) { m_drawMask = drawMask; }

bool GUIMask::GetMasking() const { return m_masking; }
bool GUIMask::GetDrawMask() const { return m_drawMask; }
