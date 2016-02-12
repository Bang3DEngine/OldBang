#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

#include "Logger.h"
#include "GLIdable.h"
#include "GLBindable.h"
#include "TextureRender.h"
#include "TextureDepth.h"

class Framebuffer : public GLBindable, public GLIdable
{
public:
    enum AttachmentType
    {
        Color = GL_COLOR_ATTACHMENT0, //For position, normal, uv, diffuse, etc.
        Depth = GL_DEPTH_ATTACHMENT   //For Depth
    };

private:

    int width, height;

    std::vector<GLuint> boundAttachments;
    std::vector<TextureRender*> textureAttachments;

protected:

    void CheckFramebufferError() const;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateTextureAttachment(int framebufferAttachmentNum,
                                 AttachmentType attachmentType = AttachmentType::Color);

    TextureRender* GetTextureAttachment(int framebufferAttachmentNum) const;

    void Resize(int width, int height);

    virtual void Bind() const override;
    virtual void UnBind() const override;
};

#endif // FRAMEBUFFER_H
