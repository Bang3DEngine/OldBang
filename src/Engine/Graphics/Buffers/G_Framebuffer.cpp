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
    glGenFramebuffers(1, &m_idGL);
}

G_Framebuffer::~G_Framebuffer()
{
    if (m_depthRenderBufferId != 0)
    {
        glDeleteRenderbuffers(1, &m_depthRenderBufferId);
    }
    glDeleteFramebuffers(1, &m_idGL);
}

void G_Framebuffer::CreateColorAttachment(AttachmentId attId,
                                        Texture2D::Format texFormat)
{
    Bind();
    GL::ClearError();
    G_RenderTexture *tex = new G_RenderTexture();
    tex->SetFormat(texFormat);
    tex->Bind();
    tex->CreateEmpty(GetWidth(), GetHeight());

    m_colorAttachmentIds.PushBack(attId);
    m_attachmentId_To_Texture.Add(attId, tex);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attId, GL_TEXTURE_2D,
                           tex->GetGLId(), 0);
    GL::CheckFramebufferError();
    GL_CheckError();
    tex->UnBind();
    UnBind();
}

void G_Framebuffer::CreateDepthRenderbufferAttachment()
{
    Bind();
    GL::ClearError();
    glGenRenderbuffers(1, &m_depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                          m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, m_depthRenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    GL::CheckFramebufferError();
    GL_CheckError();
    UnBind();
}

G_RenderTexture *G_Framebuffer::GetAttachmentTexture(AttachmentId attId) const
{
    if (!m_attachmentId_To_Texture.ContainsKey(attId)) { return nullptr; }
    return m_attachmentId_To_Texture.Get(attId);
}

void G_Framebuffer::SetAllDrawBuffers() const
{
    SetDrawBuffers(m_colorAttachmentIds);
}

void G_Framebuffer::SetDrawBuffers(const Array<AttachmentId> &attIds) const
{
    GL::ClearError();
    glDrawBuffers(attIds.Size(), (const GLenum*)(&attIds[0]));
    m_currentDrawAttachmentIds = attIds;
    GL_CheckError();
}

void G_Framebuffer::SetReadBuffer(AttachmentId attId) const
{
    GL::ClearError();
    glReadBuffer(attId);
    GL_CheckError();
}

const Array<G_Framebuffer::AttachmentId>
        &G_Framebuffer::GetCurrentDrawAttachmentIds() const
{
    return m_currentDrawAttachmentIds;
}

Color G_Framebuffer::ReadColor(int x, int y, AttachmentId attId) const
{
    Bind();
    G_RenderTexture *t = GetAttachmentTexture(attId);
    SetReadBuffer(attId);
    Color readColor;
    if (t->GetGLDataType() == GL_FLOAT)
    {
        glReadPixels(x, t->GetHeight() - y, 1, 1, t->GetGLFormat(),
                     t->GetGLDataType(), &readColor);
    }
    else
    {
        Byte bColor[4];
        glReadPixels(x, t->GetHeight() - y, 1, 1, t->GetGLFormat(),
                     t->GetGLDataType(), &bColor);
        readColor = Color(bColor[0], bColor[1], bColor[2], bColor[3]) / 255.0f;
    }
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

void G_Framebuffer::SaveToImage(AttachmentId attId, const Path &filepath,
                                bool invertY) const
{
    glFlush(); glFinish();
    G_Image img = GetAttachmentTexture(attId)->ToImage(invertY);
    img.SaveToFile(filepath);
}

void G_Framebuffer::SaveStencilToImage(const Path &filepath) const
{
    glFlush(); glFinish();
    Byte *stencilData = new Byte[GetWidth() * GetHeight()];
    glReadPixels(0, 0, GetWidth(), GetHeight(),
                 GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencilData);

    Array<Byte> bytes(GetWidth() * GetHeight() * 4);
    for (int i = 0; i < GetWidth() * GetHeight(); ++i)
    {
        bytes[i * 4 + 0] = stencilData[i];
        bytes[i * 4 + 1] = stencilData[i];
        bytes[i * 4 + 2] = stencilData[i];
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




