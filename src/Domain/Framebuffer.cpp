#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width),
                                                  height(height),
                                                  depthBufferAttachmentId(0)
{
    glGenFramebuffers(1, &idgl);
}

Framebuffer::~Framebuffer()
{
    if(depthBufferAttachmentId != 0)
        glDeleteRenderbuffers(1, &depthBufferAttachmentId);

    for(TextureRender *t : textureAttachments)
    {
        delete t;
    }

    glDeleteFramebuffers(1, &idgl);
}

void Framebuffer::CreateTextureAttachment(int framebufferAttachmentNum)
{
    Bind();
    while(int(textureAttachments.size()) <= framebufferAttachmentNum)
        textureAttachments.push_back(nullptr);

    TextureRender *tex = new TextureRender();
    tex->CreateEmpty(width, height);
    textureAttachments[framebufferAttachmentNum] = tex;

    GLuint attachment = GL_COLOR_ATTACHMENT0 + framebufferAttachmentNum;
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->GetGLId(), 0);
    boundAttachments.push_back(attachment);

    CheckFramebufferError();

    UnBind();
}

void Framebuffer::CreateDepthBufferAttachment()
{
    Bind();
    glGenRenderbuffers(1, &depthBufferAttachmentId);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferAttachmentId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferAttachmentId);

    CheckFramebufferError();
    UnBind();
}

TextureRender *Framebuffer::GetTextureAttachment(int framebufferAttachmentNum) const
{
    if(framebufferAttachmentNum >= int(textureAttachments.size())) return nullptr;
    return textureAttachments[framebufferAttachmentNum];
}

void Framebuffer::CheckFramebufferError() const
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger_Error("There was an error when creating Depth attachment for a Framebuffer.");
    }
}

void Framebuffer::Bind() const
{
    PreBind(GL_FRAMEBUFFER_BINDING);
    glBindFramebuffer(GL_FRAMEBUFFER, idgl);
    glViewport(0, 0, width, height);

    //Tell openGL which draw buffer attachments we do want to use
    glDrawBuffers(boundAttachments.size(), &boundAttachments[0]);
}

void Framebuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, PreUnBind() );
}
