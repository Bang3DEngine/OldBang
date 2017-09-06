#include "Bang/Framebuffer.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"
#include "Bang/RenderTexture.h"

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height)
{
    GL::GenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    for (auto itPair : m_attachmentId_To_Texture)
    {
        Texture2D *tex = itPair.second;
        delete tex;
    }

    if (m_depthRenderBufferId != 0)
    {
        GL::DeleteRenderBuffers(1, &m_depthRenderBufferId);
    }
    GL::DeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::CreateColorAttachment(GL::Attachment attachment,
                                        GL::ColorFormat texFormat)
{
    ASSERT(GL::IsBound(this));
    GL::ClearError();
    RenderTexture *tex = new RenderTexture();
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

void Framebuffer::CreateDepthRenderbufferAttachment()
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

RenderTexture *Framebuffer::GetAttachmentTexture(GL::Attachment attachment) const
{
    if (!m_attachmentId_To_Texture.ContainsKey(attachment)) { return nullptr; }
    return m_attachmentId_To_Texture.Get(attachment);
}

void Framebuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers(m_colorAttachmentIds);
}

void Framebuffer::SetDrawBuffers(const Array<GL::Attachment> &attachments) const
{
    ASSERT(GL::IsBound(this));
    GL::DrawBuffers(attachments);
    m_currentDrawAttachmentIds = attachments;
}

void Framebuffer::SetReadBuffer(GL::Attachment attachment) const
{
    ASSERT(GL::IsBound(this));
    GL::ReadBuffer(attachment);
}

const Array<GL::Attachment>& Framebuffer::GetCurrentDrawAttachments() const
{
    return m_currentDrawAttachmentIds;
}

Color Framebuffer::ReadColor(int x, int y, GL::Attachment attachment) const
{
    Bind();
    RenderTexture *t = GetAttachmentTexture(attachment);
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


void Framebuffer::Resize(int width, int height)
{
    m_width  = Math::Max(width,  1);
    m_height = Math::Max(height, 1);

    for (auto it : m_attachmentId_To_Texture)
    {
        RenderTexture *t = it.second;
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

int Framebuffer::GetWidth() const
{
    return m_width;
}

int Framebuffer::GetHeight() const
{
    return m_height;
}

Vector2 Framebuffer::GetSize() const
{
    return Vector2(GetWidth(), GetHeight());
}

void Framebuffer::Clear()
{
    ClearDepth(1.0f);
    ClearColor(Color::Zero);
}

void Framebuffer::ClearDepth(float clearDepth)
{
    GL::ClearDepthBuffer(clearDepth);
}

void Framebuffer::ClearColor(const Color &clearColor)
{
    SetAllDrawBuffers();
    GL::ClearColorBuffer(clearColor);
}

GL::BindTarget Framebuffer::GetGLBindTarget() const
{
    return GL::BindTarget::Framebuffer;
}

void Framebuffer::Bind() const
{
    GL::Bind(this);
    GL::SetViewport(0, 0, GetWidth(), GetHeight());
}
void Framebuffer::UnBind() const
{
    GL::UnBind(this);
}

void Framebuffer::Export(GL::Attachment attachment,
                         const Path &filepath,
                         bool invertY) const
{
    GL::Flush(); GL::Finish();
    G_Image img = GetAttachmentTexture(attachment)->ToImage(invertY);
    img.Export(filepath);
}

void Framebuffer::ExportStencil(const Path &filepath,
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

void Framebuffer::PushDrawAttachmentIds()
{
    m_latestDrawAttachmentIds = m_currentDrawAttachmentIds;
}

void Framebuffer::PopDrawAttachmentIds()
{
    SetDrawBuffers(m_latestDrawAttachmentIds);
}




