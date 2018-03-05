#include "Bang/Framebuffer.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height)
{
    GL::GenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    GL::DeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::CreateAttachment(GL::Attachment attachment,
                                   GL::ColorFormat texFormat)
{
    GLId prevBoundFB = GL::GetBoundId(GetGLBindTarget());
    Bind();

    GL_ClearError();
    RH<Texture2D> tex = Resources::Create<Texture2D>();
    tex.Get()->Bind();
    tex.Get()->SetInternalFormat(texFormat);
    tex.Get()->CreateEmpty(GetWidth(), GetHeight());
    GL_CheckError();

    SetAttachmentTexture(tex.Get(), attachment);
    tex.Get()->UnBind();

    GL::Bind(GetGLBindTarget(), prevBoundFB);
}

Texture2D* Framebuffer::GetAttachmentTexture(GL::Attachment attachment) const
{
    if (!m_attachments_To_Texture.ContainsKey(attachment)) { return nullptr; }
    return m_attachments_To_Texture.Get(attachment).Get();
}

void Framebuffer::SetAllDrawBuffers() const
{
    Array<GL::Attachment> colorAtts = m_attachments;
    colorAtts.Remove(GL::Attachment::DepthStencil);
    SetDrawBuffers(colorAtts);
}

void Framebuffer::SetDrawBuffers(const Array<GL::Attachment> &attachments) const
{
    ASSERT(GL::GetBoundId(GL::BindTarget::DrawFramebuffer) == GetGLId());
    GL::DrawBuffers(attachments);
    m_currentDrawAttachments = attachments;
}

void Framebuffer::SetReadBuffer(GL::Attachment attachment) const
{
    ASSERT(GL::GetBoundId(GL::BindTarget::ReadFramebuffer) == GetGLId());
    GL::ReadBuffer(attachment);
    m_currentReadAttachment = attachment;
}

void Framebuffer::SetAttachmentTexture(Texture2D* tex,
                                       GL::Attachment attachment)
{
    GLId prevId = GL::GetBoundId(GL::BindTarget::Framebuffer);

    m_attachments_To_Texture.Remove(attachment);

    Bind();
    GL_ClearError();
    GL::FramebufferTexture2D(GL::FramebufferTarget::ReadDraw,
                             attachment,
                             GL::TextureTarget::Texture2D,
                             tex->GetGLId());
    GL::CheckFramebufferError();
    UnBind();

    m_attachments.PushBack(attachment);
    RH<Texture2D> texRH;
    texRH.Set(tex);
    m_attachments_To_Texture.Add(attachment, texRH);

    GL::Bind(GL::BindTarget::Framebuffer, prevId);
}

GL::Attachment Framebuffer::GetCurrentReadAttachment() const
{
    return m_currentReadAttachment;
}

const Array<GL::Attachment>& Framebuffer::GetCurrentDrawAttachments() const
{
    return m_currentDrawAttachments;
}

Color Framebuffer::ReadColor(int x, int y, GL::Attachment attachment) const
{
    GLId prevFBId = GL::GetBoundId(GL::BindTarget::Framebuffer);
    Bind();
    Texture2D* t = GetAttachmentTexture(attachment);
    SetReadBuffer(attachment);
    Byte color[4] = {0,0,0,0};
    GL::ReadPixels(x, y, 1, 1,
                   GL::GetColorCompFrom(t->GetInternalFormat()),
                   t->GetInternalDataType(),
                   Cast<void*>(&color));
    Color readColor = Color(color[0], color[1], color[2], color[3]) / 255.0f;
    GL::Bind(GL::BindTarget::Framebuffer, prevFBId);
    return readColor;
}


void Framebuffer::Resize(int width, int height)
{
    m_width  = Math::Max(width,  1);
    m_height = Math::Max(height, 1);

    for (const auto &it : m_attachments_To_Texture)
    {
        Texture2D *t = it.second.Get();
        if (t) { t->Resize(m_width, m_height); }
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
}
void Framebuffer::UnBind() const
{
    GL::UnBind(this);
}

void Framebuffer::Export(GL::Attachment attachment,
                         const Path &filepath,
                         bool invertY) const
{
    ASSERT(GL::IsBound(this));

    GL::Flush(); GL::Finish();

    Imageb img = GetAttachmentTexture(attachment)->ToImage(invertY);
    img.Export(filepath);
}

template<class T>
void ExportDepthOrStencil(const Framebuffer *fb,
                          const Path &filepath,
                          bool depth,
                          int multiplier)
{
    ASSERT(GL::IsBound(fb));

    GL::Flush(); GL::Finish();

    T *data = new T[fb->GetWidth() * fb->GetHeight()];
    GL::ReadPixels(0, 0, fb->GetWidth(), fb->GetHeight(),
                   (depth ? GL::ColorComp::Depth :
                            GL::ColorComp::StencilIndex),
                   (depth ? GL::DataType::Float :
                            GL::DataType::UnsignedByte),
                   data);

    Array<Byte> bytes(fb->GetWidth() * fb->GetHeight() * 4);
    for (int i = 0; i < fb->GetWidth() * fb->GetHeight(); ++i)
    {
        bytes[i * 4 + 0] = Byte(data[i] * multiplier);
        bytes[i * 4 + 1] = Byte(data[i] * multiplier);
        bytes[i * 4 + 2] = Byte(data[i] * multiplier);
        bytes[i * 4 + 3] = Byte(255);
    }

    Imageb img = Imageb::LoadFromData(fb->GetWidth(), fb->GetHeight(), bytes);
    img.InvertVertically();
    img.Export(filepath);

    delete[] data;
}

void Framebuffer::ExportDepth(const Path &filepath) const
{
    ExportDepthOrStencil<float>(this, filepath, true, 255);
}

void Framebuffer::ExportStencil(const Path &filepath,
                                int stencilValueMultiplier) const
{
    ExportDepthOrStencil<Byte>(this, filepath, false, stencilValueMultiplier);
}

void Framebuffer::PushDrawAttachments()
{
    m_latestDrawAttachments = m_currentDrawAttachments;
}

void Framebuffer::PopDrawAttachments()
{
    SetDrawBuffers(m_latestDrawAttachments);
}
