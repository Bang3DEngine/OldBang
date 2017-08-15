#include "Bang/GUIMask.h"

#include "Bang/GL.h"

GUIMask::GUIMask() : UIGameObject("GUIMask") {}
GUIMask::~GUIMask() {}

void GUIMask::Render(RenderPass renderPass)
{
    bool canvasPass = (renderPass == RenderPass::Canvas);
    if  (canvasPass) { BeforeThisRender(); }

    UIGameObject::Render(renderPass); // Inside here, OnRender(below) is called

    if (canvasPass) { AfterChildrenRender(); }
}

void GUIMask::OnRender(RenderPass renderPass)
{
    UIGameObject::OnRender(renderPass);
    if (renderPass == RenderPass::Canvas) { BeforeChildrenRender(); }
}

void GUIMask::SetDrawMask(bool drawMask) { m_drawMask = drawMask; }
bool GUIMask::GetDrawMask() const { return m_drawMask; }

void GUIMask::BeforeThisRender()
{
    // Save values for later restoring
    m_maskRBefore = GL::IsColorMaskR();
    m_maskGBefore = GL::IsColorMaskG();
    m_maskBBefore = GL::IsColorMaskB();
    m_maskABefore = GL::IsColorMaskA();
    m_stencilTestBefore  = GL::IsStencilTest();
    m_stencilWriteBefore = GL::IsStencilWrite();
    m_stencilOpBefore = GL::GetStencilOp();

    // Will this mask be drawn?
    GL::SetColorMask(m_drawMask, m_drawMask, m_drawMask, m_drawMask);
    GL::SetStencilOp( GL_INCR );
    GL::SetStencilTest(true);  // Only increment once
    GL::SetStencilWrite(true); // This rendering will write to the stencil
}

void GUIMask::BeforeChildrenRender()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskRBefore, m_maskGBefore, m_maskBBefore, m_maskABefore);

    // Test and write for current stencil value + 1
    GL::SetStencilValue( GL::GetStencilValue() + 1 );
    GL::SetStencilOp(m_stencilOpBefore);
    GL::SetStencilWrite(m_stencilWriteBefore); // Restore stencil write
}

void GUIMask::AfterChildrenRender()
{
    // Restore stencil as it was before
    GL::SetStencilValue( GL::GetStencilValue() - 1 );
    GL::SetStencilTest(m_stencilTestBefore);
}
