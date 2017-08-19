#include "Bang/GUIMask.h"

#include "Bang/GL.h"

GUIMask::GUIMask() : UIGameObject("GUIMask") {}
GUIMask::~GUIMask() {}

void GUIMask::Render(RenderPass renderPass)
{
    bool canvasPass = (renderPass == RenderPass::Canvas);
    if  (canvasPass) { BeforeThisRender(); }

    UIGameObject::Render(renderPass); // Inside here, OnRender(below) is called

    if (canvasPass) { AfterChildrenRender(renderPass); }
}

void GUIMask::OnRender(RenderPass renderPass)
{
    UIGameObject::OnRender(renderPass);
    bool canvasPass = (renderPass == RenderPass::Canvas);
    if (canvasPass) { BeforeChildrenRender(); }
}

void GUIMask::SetMasking(bool maskEnabled) { m_masking = maskEnabled; }
void GUIMask::SetDrawMask(bool drawMask) { m_drawMask = drawMask; }

bool GUIMask::GetMasking() const { return m_masking; }
bool GUIMask::GetDrawMask() const { return m_drawMask; }

#include "Bang/G_GBuffer.h"
#include "Bang/GraphicPipeline.h"
void GUIMask::BeforeThisRender()
{
    GraphicPipeline::GetActive()->GetGBuffer()->SaveStencilToImage(
                Path("/home/sephirot47/Bang/tmp/" + GetInstanceId() + "_A.png"));

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

void GUIMask::BeforeChildrenRender()
{
    // Restore color mask for children
    GL::SetColorMask(m_maskRBefore, m_maskGBefore, m_maskBBefore, m_maskABefore);

    if (GetMasking())
    {
        // Test and write for current stencil value + 1
        GL::SetStencilValue( GL::GetStencilValue() + 1 );
        GL::SetStencilOp(m_stencilOpBefore);
        GL::SetStencilTest(true);
        GL::SetStencilWrite(m_stencilWriteBefore); // Restore stencil write
    }
}

void GUIMask::AfterChildrenRender(RenderPass renderPass)
{
    if (GetMasking())
    {
        GraphicPipeline::GetActive()->GetGBuffer()->SaveStencilToImage(
                    Path("/home/sephirot47/Bang/tmp/" + GetInstanceId() + "_B.png"));

        // Restore stencil as it was before, decrementing marked mask pixels
        GL::SetColorMask(false, false, false, false);
        GL::SetStencilTest(false);
        GL::SetStencilWrite(true);
        GL::SetStencilOp(GL_DECR);

        UIGameObject::OnRender(renderPass);

        GraphicPipeline::GetActive()->GetGBuffer()->SaveStencilToImage(
                    Path("/home/sephirot47/Bang/tmp/" + GetInstanceId() + "_C.png"));

        GL::SetColorMask(m_maskRBefore, m_maskGBefore,
                         m_maskBBefore, m_maskABefore);
        GL::SetStencilValue( GL::GetStencilValue() - 1 );
        GL::SetStencilOp(m_stencilOpBefore);
        GL::SetStencilWrite(m_stencilWriteBefore);
        GL::SetStencilTest(m_stencilTestBefore);
    }
}
