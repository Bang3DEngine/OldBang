#include "Bang/GBuffer.h"

#include "Bang/VAO.h"
#include "Bang/Debug.h"
#include "Bang/Mesh.h"
#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/Color.h"
#include "Bang/Array.h"
#include "Bang/Input.h"
#include "Bang/Camera.h"
#include "Bang/Screen.h"
#include "Bang/Material.h"
#include "Bang/Transform.h"
#include "Bang/MeshFactory.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AssetsManager.h"
#include "Bang/RenderTexture.h"
#include "Bang/GraphicPipeline.h"

GBuffer::GBuffer(int width, int height) : Framebuffer(width, height)
{
    CreateColorAttachment( AttachmentId( AttNormal  ),
                           Texture::Format::RGBA_Byte8 );
    CreateColorAttachment( AttachmentId( AttDiffuse ),
                           Texture::Format::RGBA_Float16 );
    CreateColorAttachment( AttachmentId( AttMisc    ),
                           Texture::Format::RGBA_Float32  );
    CreateColorAttachment( AttachmentId( AttColor  ),
                           Texture::Format::RGBA_Float16  );
    CreateColorAttachment( AttachmentId( AttColorRead  ),
                           Texture::Format::RGBA_Float16  );
    CreateDepthRenderbufferAttachment();

    m_normalTexture    = GetAttachmentTexture(AttNormal);
    m_diffuseTexture   = GetAttachmentTexture(AttDiffuse);
    m_miscTexture      = GetAttachmentTexture(AttMisc);
    m_colorTexture     = GetAttachmentTexture(AttColor);
    m_colorReadTexture = GetAttachmentTexture(AttColorRead);
}

GBuffer::~GBuffer()
{
}

void GBuffer::BindTextureBuffersTo(ShaderProgram *sp) const
{
    // Color Attachments bindings as Shader Inputs
    sp->SetTexture("B_GTex_Normal",    m_normalTexture);
    sp->SetTexture("B_GTex_DiffColor", m_diffuseTexture);
    sp->SetTexture("B_GTex_Misc",      m_miscTexture);

    sp->SetTexture("B_GTex_Color",
                   m_willReadFromColorRead ? m_colorReadTexture :
                                             m_colorTexture);
    m_willReadFromColorRead = false;
}


void GBuffer::ApplyPass(ShaderProgram *sp,
                        bool prepareReadFromColorBuffer,
                        const Rect &mask)
{
    sp->SetVec2("B_ScreenSize", Screen::GetSize());

    bool prevStencilWrite = m_stencilWrite;
    SetStencilWrite(false);

    Bind();
    BindTextureBuffersTo(sp);

    if (prepareReadFromColorBuffer)
    {
        PrepareColorReadBuffer(sp);
    }

    SetColorDrawBuffer();
    GraphicPipeline::GetActive()->ApplyScreenPass(sp, mask);

    UnBind();

    SetStencilWrite(prevStencilWrite);
}

void GBuffer::RenderToScreen(GBuffer::AttachmentId attId)
{
    // Assumes gbuffer is not bound, hence directly writing to screen
    Texture *tex = GetAttachmentTexture(attId); ASSERT(tex);
    GraphicPipeline::GetActive()->RenderToScreen(tex);
}

void GBuffer::RenderToScreen()
{
    RenderToScreen(GBuffer::AttColor);
}

void GBuffer::PrepareColorReadBuffer(ShaderProgram *sp)
{
    SetReadBuffer(AttColor);
    SetDrawBuffers({AttColorRead});
    glBlitFramebuffer(0, 0, GetWidth(), GetHeight(),
                      0, 0, GetWidth(), GetHeight(),
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);

    m_willReadFromColorRead = true;
}

void GBuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc,
                    GBuffer::AttColor});
}

void GBuffer::SetAllDrawBuffersExceptColor()
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse, GBuffer::AttMisc});
}

void GBuffer::SetColorDrawBuffer()
{
    SetDrawBuffers({GBuffer::AttColor});
}

void GBuffer::SetStencilWrite(bool writeEnabled)
{
    // Replace is to ref value 1
    if (m_stencilWrite != writeEnabled)
    {
        glStencilOp(writeEnabled ? GL_REPLACE : GL_KEEP, GL_KEEP, GL_KEEP);
    }
    m_stencilWrite = writeEnabled;
}

void GBuffer::SetStencilTest(bool testEnabled)
{
    if (m_stencilTest != testEnabled)
    {
        glStencilFunc(testEnabled ? GL_GEQUAL : GL_ALWAYS, 1, 0xFF);
    }
    m_stencilTest = testEnabled;
}

void GBuffer::ClearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}

void GBuffer::ClearDepth(float clearDepth)
{
    Framebuffer::ClearDepth(clearDepth); // Clear typical depth buffer
    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color::One, false, false, true, false);
}

void GBuffer::ClearBuffersAndBackground(const Color &backgroundColor,
                                        const Color &clearValue)
{
    SetDrawBuffers({GBuffer::AttNormal, GBuffer::AttDiffuse});
    GL::ClearColorBuffer(clearValue);

    SetDrawBuffers({GBuffer::AttMisc});
    GL::ClearColorBuffer(Color(0,0,1,0));

    GL::ClearDepthBuffer(1.0f);

    SetColorDrawBuffer();
    GL::ClearColorBuffer(backgroundColor);
}
