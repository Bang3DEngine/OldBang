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
    Bind();
    GL::ClearError();
    G_RenderTexture *tex = new G_RenderTexture();
    tex->SetInternalFormat(texFormat);
    tex->Bind();
    tex->CreateEmpty(GetWidth(), GetHeight());

    m_colorAttachmentIds.PushBack(attachment);
    m_attachmentId_To_Texture.Add(attachment, tex);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
                           GLCAST(attachment),
                           GLCAST(GL::TextureTarget::Texture2D),
                           tex->GetGLId(),
                           0);
    GL::CheckFramebufferError();
    GL_CheckError();
    tex->UnBind();
    UnBind();
}

void G_Framebuffer::CreateDepthRenderbufferAttachment()
{
    Bind();
    GL::ClearError();
    GL::GenRenderBuffers(1, &m_depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GLCAST(GL::Attachment::DepthStencil),
                              GL_RENDERBUFFER, m_depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GL::CheckFramebufferError();
    GL_CheckError();
    UnBind();
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
    GL::ClearError();
    glDrawBuffers(attachments.Size(), (const GLenum*)(&attachments[0]));
    m_currentDrawAttachmentIds = attachments;
    GL_CheckError();
}

void G_Framebuffer::SetReadBuffer(GL::Attachment attachment) const
{
    GL::ClearError();
    glReadBuffer( GLCAST(attachment) );
    GL_CheckError();
}

const Array<GL::Attachment>& G_Framebuffer::GetCurrentDrawAttachmentIds() const
{
    return m_currentDrawAttachmentIds;
}

Color G_Framebuffer::ReadColor(int x, int y, GL::Attachment attachment) const
{
    Bind();
    G_RenderTexture *t = GetAttachmentTexture(attachment);
    SetReadBuffer(attachment);
    Byte color[4];
    glReadPixels(x, t->GetHeight() - y,
                 1, 1,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &color);
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
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                              m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
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
    GL::SetViewport(0, 0, m_width, m_height);
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
    img.SaveToFile(filepath);
}

void G_Framebuffer::SaveStencilToImage(const Path &filepath,
                                       int stencilValueMultiplier) const
{
    GL::Flush(); GL::Finish();
    Byte *stencilData = new Byte[GetWidth() * GetHeight()];
    glReadPixels(0, 0, GetWidth(), GetHeight(),
                 GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData);

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
    img.SaveToFile(filepath);

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




