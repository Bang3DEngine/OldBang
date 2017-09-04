#include "Bang/G_Framebuffer.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"
#include "Bang/G_RenderTexture.h"

G_Framebuffer::G_Framebuffer(int width, int height) : m_width(width),
                                                      m_height(height)
{
    GL::GenFramebuffers(1, &m_idGL);
}

G_Framebuffer::~G_Framebuffer()
{
    if (m_depthRenderBufferId != 0)
    {
        GL::DeleteRenderBuffers(1, &m_depthRenderBufferId);
    }
    GL::DeleteFramebuffers(1, &m_idGL);
}

void G_Framebuffer::CreateColorAttachment(GL::Attachment attachment,
                                          GL::ColorFormat texFormat)
{
    ASSERT(GL::IsBound(this));
    GL::ClearError();
    G_RenderTexture *tex = new G_RenderTexture();
    tex->SetInternalFormat(texFormat);
    tex->Bind();
    tex->CreateEmpty(GetWidth(), GetHeight());
    GL_CheckError();

    m_colorAttachmentIds.PushBack(attachment);
    m_attachmentId_To_Texture.Add(attachment, tex);

    GL::ClearError();
    GL::FramebufferTexture2D(GL::FramebufferTarget::ReadDraw,
                             attachment,
                             GL::TextureTarget::Texture2D,
                             tex->GetGLId());
    GL::CheckFramebufferError();
    tex->UnBind();
}

void G_Framebuffer::CreateDepthRenderbufferAttachment()
{
    ASSERT(GL::IsBound(this));
    GL::GenRenderBuffers(1, &m_depthRenderBufferId);
    GL::BindRenderbuffer(GL::RenderbufferTarget::Renderbuffer,
                         m_depthRenderBufferId);
    GL::RenderbufferStorage(GL::RenderbufferTarget::Renderbuffer,
                            GL::RenderbufferFormat::Depth24_Stencil8,
                            m_width, m_height);
    GL::FramebufferRenderbuffer(GL::FramebufferTarget::ReadDraw,
                                GL::Attachment::DepthStencil,
                                GL::RenderbufferTarget::Renderbuffer,
                                m_depthRenderBufferId);
    GL::BindRenderbuffer(GL::RenderbufferTarget::Renderbuffer, 0);
    GL::CheckFramebufferError();
}

G_RenderTexture *G_Framebuffer::GetAttachmentTexture(GL::Attachment attachment) const
{
    if (!m_attachmentId_To_Texture.ContainsKey(attachment)) { return nullptr; }
    return m_attachmentId_To_Texture.Get(attachment);
}

void G_Framebuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers(m_colorAttachmentIds);
}

void G_Framebuffer::SetDrawBuffers(const Array<GL::Attachment> &attachments) const
{
    ASSERT(GL::IsBound(this));
    GL::DrawBuffers(attachments);
    m_currentDrawAttachmentIds = attachments;
}

void G_Framebuffer::SetReadBuffer(GL::Attachment attachment) const
{
    ASSERT(GL::IsBound(this));
    GL::ReadBuffer(attachment);
}

const Array<GL::Attachment>& G_Framebuffer::GetCurrentDrawAttachments() const
{
    return m_currentDrawAttachmentIds;
}

Color G_Framebuffer::ReadColor(int x, int y, GL::Attachment attachment) const
{
    Bind();
    G_RenderTexture *t = GetAttachmentTexture(attachment);
    SetReadBuffer(attachment);
    Byte color[4];
    GL::ReadPixels(x, t->GetHeight() - y, 1, 1,
                   GL::GetColorCompFrom(t->GetInternalFormat()),
                   t->GetInternalDataType(),
                   SCAST<void*>(&color));
    Color readColor = Color(color[0], color[1], color[2], color[3]) / 255.0f;
    UnBind();
    return readColor;
}


void G_Framebuffer::Resize(int width, int height)
{
    m_width  = Math::Max(width,  1);
    m_height = Math::Max(height, 1);

    for (auto it : m_attachmentId_To_Texture)
    {
        G_RenderTexture *t = it.second;
        if (t)
        {
            GL::ClearError();
            t->Resize(m_width, m_height);
        }
    }

    if (m_depthRenderBufferId != 0)
    {
        GL::ClearError();
        //TODO:  respect former bindings of renderbuffers
        GL::BindRenderbuffer(GL::RenderbufferTarget::Renderbuffer,
                             m_depthRenderBufferId);
        GL::RenderbufferStorage(GL::RenderbufferTarget::Renderbuffer,
                                GL::RenderbufferFormat::Depth24_Stencil8,
                                m_width, m_height);
        GL::BindRenderbuffer(GL::RenderbufferTarget::Renderbuffer, 0);
    }
}

int G_Framebuffer::GetWidth() const
{
    return m_width;
}

int G_Framebuffer::GetHeight() const
{
    return m_height;
}

Vector2 G_Framebuffer::GetSize() const
{
    return Vector2(GetWidth(), GetHeight());
}

void G_Framebuffer::Clear()
{
    ClearDepth(1.0f);
    ClearColor(Color::Zero);
}

void G_Framebuffer::ClearDepth(float clearDepth)
{
    GL::ClearDepthBuffer(clearDepth);
}

void G_Framebuffer::ClearColor(const Color &clearColor)
{
    SetAllDrawBuffers();
    GL::ClearColorBuffer(clearColor);
}

GL::BindTarget G_Framebuffer::GetGLBindTarget() const
{
    return GL::BindTarget::Framebuffer;
}

void G_Framebuffer::Bind() const
{
    GL::Bind(this);
    GL::SetViewport(0, 0, GetWidth(), GetHeight());
}
void G_Framebuffer::UnBind() const
{
    GL::UnBind(this);
}

void G_Framebuffer::SaveToImage(GL::Attachment attachment,
                                const Path &filepath,
                                bool invertY) const
{
    GL::Flush(); GL::Finish();
    G_Image img = GetAttachmentTexture(attachment)->ToImage(invertY);
    img.Export(filepath);
}

void G_Framebuffer::SaveStencilToImage(const Path &filepath,
                                       int stencilValueMultiplier) const
{
    GL::Flush(); GL::Finish();
    Byte *stencilData = new Byte[GetWidth() * GetHeight()];
    GL::ReadPixels(0, 0, GetWidth(), GetHeight(),
                   GL::ColorComp::StencilIndex,
                   GL::DataType::UnsignedByte, stencilData);

    Array<Byte> bytes(GetWidth() * GetHeight() * 4);
    for (int i = 0; i < GetWidth() * GetHeight(); ++i)
    {
        bytes[i * 4 + 0] = stencilData[i] * stencilValueMultiplier;
        bytes[i * 4 + 1] = stencilData[i] * stencilValueMultiplier;
        bytes[i * 4 + 2] = stencilData[i] * stencilValueMultiplier;
        bytes[i * 4 + 3] = 255;
    }

    G_Image img = G_Image::LoadFromData(GetWidth(), GetHeight(), bytes);
    img.InvertVertically();
    img.Export(filepath);

    delete stencilData;
}

void G_Framebuffer::PushDrawAttachmentIds()
{
    m_latestDrawAttachmentIds = m_currentDrawAttachmentIds;
}

void G_Framebuffer::PopDrawAttachmentIds()
{
    SetDrawBuffers(m_latestDrawAttachmentIds);
}




