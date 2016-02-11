#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

#include "Logger.h"
#include "Texture2D.h"
#include "GLIdable.h"
#include "GLBindable.h"

class Framebuffer : public GLBindable, public GLIdable
{
private:

    int width, height;
    GLuint depthBufferAttachmentId;

    std::vector<GLenum> boundAttachments;
    std::vector<Texture2D*> textureAttachments;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateTextureAttachment(int framebufferAttachmentNum);
    void CreateDepthBufferAttachment();

    void Ready();


    void Bind() const override;
    void UnBind() const override;
};

#endif // FRAMEBUFFER_H
