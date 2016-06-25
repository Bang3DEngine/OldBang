#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width),
                                                  height(height),
                                                  depthBufferAttachmentId(0)
{
    glGenFramebuffers(1, &idgl);
}

Framebuffer::~Framebuffer()
{
    for(TextureRender *t : textureAttachments)
    {
        delete t;
    }

    if(depthBufferAttachmentId != 0)
        glDeleteRenderbuffers(1, &depthBufferAttachmentId);

    glDeleteFramebuffers(1, &idgl);
}

void Framebuffer::CreateTextureAttachment(int framebufferAttachmentNum, AttachmentType attachmentType)
{
    while(int(textureAttachments.size()) <= framebufferAttachmentNum)
        textureAttachments.push_back(nullptr);

    Bind();

    //Create texture
    TextureRender *tex = new TextureRender();
    tex->CreateEmpty(width, height);
    textureAttachments[framebufferAttachmentNum] = tex;


    //Attach it to framebuffer itself
    GLuint attachment;
    if(attachmentType == AttachmentType::Color)
    {
        attachment = GL_COLOR_ATTACHMENT0 + framebufferAttachmentNum;
    }
    else
    {
        attachment = GL_DEPTH_ATTACHMENT;
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->GetGLId(), 0);
    boundAttachments.push_back(attachment);
    //


    CheckFramebufferError();

    UnBind();
}


void Framebuffer::CreateDepthBufferAttachment()
{
    Bind();
    glGenRenderbuffers(1, &depthBufferAttachmentId);
    //TODO:  respect former bindings of renderbuffers
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

void Framebuffer::SetReadBuffer(int attachmentId) const
{
    Bind();
    glReadBuffer(boundAttachments[attachmentId]);
    UnBind();
}

//TODO: Fix Bind and UnBind, if you call it repeatedly it does not work
Vector3 Framebuffer::ReadPixel(int x, int y, int attachmentId) const
{
    unsigned char color[3];
    SetReadBuffer(attachmentId);
    Bind();
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &color[0]);
    UnBind();

    return Vector3(color[0], color[1], color[2]);
}

void Framebuffer::Resize(int width, int height)
{
    this->width = width;
    this->height = height;
    for(Texture *t : textureAttachments)
    {
        if(t )
        {
            t->Resize(width, height);
        }
    }

    if(depthBufferAttachmentId != 0)
    {
        //TODO:  respect former bindings of renderbuffers
        glBindRenderbuffer(GL_RENDERBUFFER, depthBufferAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    }
}

void Framebuffer::Clear() const
{
    Bind();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    UnBind();
}

void Framebuffer::ClearDepth() const
{
    Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    UnBind();
}

void Framebuffer::ClearColor() const
{
    Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    UnBind();
}

void Framebuffer::CheckFramebufferError() const
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger_Error("There was an error when creating an attachment for a Framebuffer.");
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
    glBindFramebuffer(GL_FRAMEBUFFER, PreUnBind(GL_FRAMEBUFFER_BINDING) );
}




