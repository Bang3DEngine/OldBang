#include "Bang/GUIMask.h"

#include "Bang/GL.h"

GUIMask::GUIMask() : UIGameObject("GUIMask") {}
GUIMask::~GUIMask() {}

void GUIMask::Render(RenderPass renderPass)
{
    bool canvasPass = IsCanvasPass(renderPass);
    if  (canvasPass) { BeforeThisRender(); }

    UIGameObject::Render(renderPass); // Inside here, OnRender(below) is called

    if (canvasPass) { AfterChildrenRender(); }
}

void GUIMask::OnRender(RenderPass renderPass)
{
    UIGameObject::OnRender(renderPass);

    bool canvasPass = IsCanvasPass(renderPass);
    if (canvasPass) { BeforeChildrenRender(); }
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

    // Will this mask be drawn?
    GL::SetColorMask(m_drawMask, m_drawMask, m_drawMask, m_drawMask);
    GL::SetStencilWrite(true); // This rendering will write to the stencil
}

void GUIMask::BeforeChildrenRender()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskRBefore, m_maskGBefore, m_maskBBefore, m_maskABefore);
    GL::SetStencilTest(true); // Make sure children use stencil test
}

void GUIMask::AfterChildrenRender()
{
    // Restore stencil write/test as it was before
    GL::SetStencilTest(m_stencilTestBefore);
    GL::SetStencilWrite(m_stencilWriteBefore);
    GL::ClearStencilBuffer();
}

bool GUIMask::IsCanvasPass(RenderPass rp)
{
    return (rp == RenderPass::Canvas || rp == RenderPass::Canvas_Selection);
}
