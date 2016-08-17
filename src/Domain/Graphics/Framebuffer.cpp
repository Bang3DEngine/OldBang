#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height),
                                                  m_depthBufferAttachmentId(0)
{
    glGenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    for (TextureRender *t : m_textureAttachments)
    {
        delete t;
    }

    if (m_depthBufferAttachmentId != 0)
        glDeleteRenderbuffers(1, &m_depthBufferAttachmentId);

    glDeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::CreateColorAttachment(int framebufferAttachmentNum,
                                        GLint glInternalFormat, GLint glFormat, GLint glInternalType,
                                        bool depthAttachment)
{
    while (int(m_textureAttachments.size()) <= framebufferAttachmentNum)
        m_textureAttachments.push_back(nullptr);

    Bind();

    //Create texture
    TextureRender *tex = new TextureRender();

    if (glInternalFormat != -1)
    {
        tex->SetGLInternalFormat(glInternalFormat);
    }

    if (glFormat != -1)
    {
        tex->SetGLFormat(glFormat);
    }

    if (glInternalType != -1)
    {
        tex->SetGLInternalType(glInternalType);
    }

    tex->CreateEmpty(m_width, m_height);
    m_textureAttachments[framebufferAttachmentNum] = tex;


    //Attach it to framebuffer itself
    GLuint attachment = GL_COLOR_ATTACHMENT0 + framebufferAttachmentNum;
    if (depthAttachment) attachment = GL_DEPTH_ATTACHMENT;
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->GetGLId(), 0);
    m_boundAttachments.push_back(attachment);
    //

    CheckFramebufferError();

    UnBind();
}


void Framebuffer::CreateDepthBufferAttachment()
{
    Bind();
    glGenRenderbuffers(1, &m_depthBufferAttachmentId);
    //TODO:  respect former bindings of renderbuffers
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferAttachmentId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferAttachmentId);

    CheckFramebufferError();
    UnBind();
}

TextureRender *Framebuffer::GetTextureAttachment(int framebufferAttachmentNum) const
{
    if (framebufferAttachmentNum >= int(m_textureAttachments.size())) return nullptr;
    return m_textureAttachments[framebufferAttachmentNum];
}

void Framebuffer::SetAllDrawBuffers() const
{
    std::vector<int> attachmentIds;
    for (int i = 0; i < m_boundAttachments.size(); ++i)
    {
        attachmentIds.push_back(i);
    }
    SetDrawBuffers(attachmentIds);
}

void Framebuffer::SetDrawBuffers(const std::vector<int> &attachmentIds) const
{
    Bind();
    std::vector<GLuint> drawBuffers;
    for (GLint att : attachmentIds)
    {
        drawBuffers.push_back(m_boundAttachments[att]);
    }

    glDrawBuffers(drawBuffers.size(), &drawBuffers[0]);
    CheckFramebufferError();
    UnBind();
}

void Framebuffer::SetReadBuffer(GLuint attachmentId) const
{
    Bind();
    glReadBuffer(m_boundAttachments[attachmentId]);
    CheckFramebufferError();
    UnBind();
}

//TODO: Fix Bind and UnBind, if you call it repeatedly it does not work
Color Framebuffer::ReadColor255(int x, int y, int attachmentId, int glFormat, int glType) const
{
    unsigned char color[4];
    SetReadBuffer(attachmentId);
    Bind();
    glReadPixels(x, y, 1, 1, glFormat, glType, &color[0]);
    UnBind();

    return Color(color[0], color[1], color[2], color[3]);
}

Color Framebuffer::ReadColor(int x, int y, int attachmentId, int glFormat, int glType) const
{
    float color[4];
    SetReadBuffer(attachmentId);
    Bind();
    glReadPixels(x, y, 1, 1, glFormat, glType, &color[0]);
    UnBind();

    return Color(color[0], color[1], color[2], color[3]);
}

float Framebuffer::ReadFloat(int x, int y, int attachmentId, int glFormat, int glType) const
{
    float f = 0.0f;
    Bind();
    SetReadBuffer(attachmentId);
    glReadPixels(x, y, 1, 1, glFormat, glType, &f);
    UnBind();
    return f;
}

float Framebuffer::ReadDepth(int x, int y) const
{
    float d = 0.0f;
    Bind();
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &d);
    UnBind();
    return d;
}

void Framebuffer::Resize(int width, int height)
{
    this->m_width = width;
    this->m_height = height;
    for (Texture *t : m_textureAttachments)
    {
        if (t )
        {
            t->Resize(width, height);
        }
    }

    if (m_depthBufferAttachmentId != 0)
    {
        //TODO:  respect former bindings of renderbuffers
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    }
}

void Framebuffer::Clear() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    UnBind();
}

void Framebuffer::ClearDepth() const
{
    Bind();
    SetAllDrawBuffers();
    glClear(GL_DEPTH_BUFFER_BIT);
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
        Logger_Error("There was an error when creating an attachment for a Framebuffer.");
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
    glBindFramebuffer(GL_FRAMEBUFFER, PreUnBind(GL_FRAMEBUFFER_BINDING) );
}




