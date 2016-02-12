#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width),
                                                  height(height)
{
    glGenFramebuffers(1, &idgl);
}

Framebuffer::~Framebuffer()
{
    for(TextureRender *t : textureAttachments)
    {
        delete t;
    }

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

TextureRender *Framebuffer::GetTextureAttachment(int framebufferAttachmentNum) const
{
    if(framebufferAttachmentNum >= int(textureAttachments.size())) return nullptr;
    return textureAttachments[framebufferAttachmentNum];
}

void Framebuffer::Resize(int width, int height)
{
    this->width = width;
    this->height = height;
    for(Texture *t : textureAttachments)
    {
        if(t != nullptr)
        {
            t->Resize(width, height);
        }
    }
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
