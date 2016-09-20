#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height),
                                                  m_depthStencilAttachmentId(0)
{
    glGenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    for (auto it : m_attachmentId_To_Texture)
    {
        TextureRender *t = it.second;
        delete t;
    }

    if (m_depthStencilAttachmentId != 0)
    {
        glDeleteRenderbuffers(1, &m_depthStencilAttachmentId);
    }

    glDeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::SetColorAttachment(int attachmentId, TextureRender *tex)
{
    tex->CreateEmpty(GetWidth(), GetHeight());
    GLuint glAttachment = GL_COLOR_ATTACHMENT0 + m_attachmentId_To_Texture.Size();
    m_attachmentId_To_Texture[attachmentId] = tex;
    m_attachmentId_To_GLAttachment[attachmentId] = glAttachment;

    //Attach it to framebuffer
    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment,
                           GL_TEXTURE_2D, tex->GetGLId(), 0);
    CheckFramebufferError();
    UnBind();
    //
}

void Framebuffer::CreateDepthStencilRenderbufferAttachment()
{
    Bind();
    glGenRenderbuffers(1, &m_depthStencilAttachmentId);
    //TODO:  respect former bindings of renderbuffers
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilAttachmentId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilAttachmentId);

    CheckFramebufferError();
    UnBind();
}

TextureRender *Framebuffer::GetColorAttachment(int attachmentId) const
{
    if (!m_attachmentId_To_Texture.ContainsKey(attachmentId))
    {
        return nullptr;
    }
    return m_attachmentId_To_Texture.Get(attachmentId);
}

void Framebuffer::SetAllDrawBuffers() const
{
    Array<int> attachmentIds;
    for (int i = 0; i < m_attachmentId_To_GLAttachment.Size(); ++i)
    {
        attachmentIds.PushBack(i);
    }
    SetDrawBuffers(attachmentIds);
}

void Framebuffer::SetDrawBuffers(const Array<int> &attachmentIds) const
{
    Bind();
    m_currentDrawAttachmentIds = attachmentIds;

    Array<GLenum> drawBuffers;
    for (int attId : attachmentIds)
    {
        drawBuffers.PushBack(m_attachmentId_To_GLAttachment.Get(attId));
    }

    glDrawBuffers(drawBuffers.Size(), &drawBuffers[0]);
    CheckFramebufferError();
    UnBind();
}

void Framebuffer::SetReadBuffer(int attachmentId) const
{
    Bind();
    glReadBuffer(m_attachmentId_To_GLAttachment.Get(attachmentId));
    CheckFramebufferError();
    UnBind();
}

const Array<int> &Framebuffer::GetCurrentDrawAttachmentIds() const
{
    return m_currentDrawAttachmentIds;
}

Color Framebuffer::ReadColor(int x, int y, int attachmentId) const
{
    Bind();
    Color c;
    TextureRender *t = GetColorAttachment(attachmentId);
    SetReadBuffer(attachmentId);
    glReadPixels(x, y, 1, 1, t->GetGLFormat(), t->GetGLType(), &c);
    UnBind();
    return c;
}


void Framebuffer::Resize(int width, int height)
{
    m_width = width;
    m_height = height;
    for (auto it : m_attachmentId_To_Texture)
    {
        TextureRender *t = it.second;
        if (t)
        {
            t->Resize(width, height);
        }
    }

    if (m_depthStencilAttachmentId != 0)
    {
        //TODO:  respect former bindings of renderbuffers
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
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

void Framebuffer::Clear() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_STENCIL_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT   |
            GL_COLOR_BUFFER_BIT);
    UnBind();
}

void Framebuffer::ClearDepth() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_DEPTH_BUFFER_BIT);
    UnBind();
}

void Framebuffer::ClearStencil() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_STENCIL_BUFFER_BIT);
    UnBind();
}


void Framebuffer::ClearColor() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_COLOR_BUFFER_BIT);
    UnBind();
}

void Framebuffer::CheckFramebufferError() const
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Debug_Error("There was an error when creating an attachment for a Framebuffer.");
    }
}

void Framebuffer::Bind() const
{
    PreBind(GL_FRAMEBUFFER_BINDING);
    glBindFramebuffer(GL_FRAMEBUFFER, m_idGL);
    glViewport(0, 0, m_width, m_height);
}

void Framebuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, PreUnBind(GL_FRAMEBUFFER_BINDING));
}




