#include "Bang/Framebuffer.h"

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "Bang/GL.h"
#include "Bang/Math.h"
#include "Bang/Vector2.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height)
{
    GL::GenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    for (auto itPair : m_attachments_To_Texture)
    {
        Texture2D *tex = itPair.second;
        delete tex;
    }
    GL::DeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::CreateAttachment(GL::Attachment attachment,
                                   GL::ColorFormat texFormat)
{
    ASSERT(GL::IsBound(this));
    GL::ClearError();
    Texture2D *tex = new Texture2D();
    tex->Bind();
    tex->SetInternalFormat(texFormat);
    tex->CreateEmpty(GetWidth(), GetHeight());
    GL_CheckError();

    m_attachments.PushBack(attachment);
    m_attachments_To_Texture.Add(attachment, tex);

    SetAttachmentTexture(tex, attachment);
    tex->UnBind();
}

Texture2D *Framebuffer::GetAttachmentTexture(GL::Attachment attachment) const
{
    if (!m_attachments_To_Texture.ContainsKey(attachment)) { return nullptr; }
    return m_attachments_To_Texture.Get(attachment);
}

void Framebuffer::SetAllDrawBuffers() const
{
    Array<GL::Attachment> colorAtts = m_attachments;
    colorAtts.Remove(GL::Attachment::DepthStencil);
    SetDrawBuffers(colorAtts);
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

void Framebuffer::SetAttachmentTexture(Texture2D *tex,
                                       GL::Attachment attachment)
{
    GLId prevId = GL::GetBoundId(GL::BindTarget::Framebuffer);

    Bind();
    GL::ClearError();
    GL::FramebufferTexture2D(GL::FramebufferTarget::ReadDraw,
                             attachment,
                             GL::TextureTarget::Texture2D,
                             tex->GetGLId());
    GL::CheckFramebufferError();
    UnBind();

    if (prevId > 0) { GL::Bind(GL::BindTarget::Framebuffer, prevId); }
}

const Array<GL::Attachment>& Framebuffer::GetCurrentDrawAttachments() const
{
    return m_currentDrawAttachmentIds;
}

#include "Bang/Input.h"
Color Framebuffer::ReadColor(int x, int y, GL::Attachment attachment) const
{
    GLId prevFBId = GL::GetBoundId(GL::BindTarget::Framebuffer);
    Bind();
    Texture2D *t = GetAttachmentTexture(attachment);
    SetReadBuffer(attachment);
    Byte color[4] = {0,0,0,0};
    GL::ReadPixels(x, y, 1, 1,
                   GL::GetColorCompFrom(t->GetInternalFormat()),
                   t->GetInternalDataType(),
                   SCAST<void*>(&color));
    Color readColor = Color(color[0], color[1], color[2], color[3]) / 255.0f;
    GL::Bind(GL::BindTarget::Framebuffer, prevFBId);
    return readColor;
}


void Framebuffer::Resize(int width, int height)
{
    m_width  = Math::Max(width,  1);
    m_height = Math::Max(height, 1);

    for (auto it : m_attachments_To_Texture)
    {
        Texture2D *t = it.second;
        if (t)
        {
            GL::ClearError();
            t->Resize(m_width, m_height);
        }
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
    GL::Flush(); GL::Finish();
    Imageb img = GetAttachmentTexture(attachment)->ToImage(invertY);
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

    Imageb img = Imageb::LoadFromData(GetWidth(), GetHeight(), bytes);
    img.InvertVertically();
    img.Export(filepath);

    delete[] stencilData;
}

void Framebuffer::PushDrawAttachments()
{
    m_latestDrawAttachmentIds = m_currentDrawAttachmentIds;
}

void Framebuffer::PopDrawAttachments()
{
    SetDrawBuffers(m_latestDrawAttachmentIds);
}
