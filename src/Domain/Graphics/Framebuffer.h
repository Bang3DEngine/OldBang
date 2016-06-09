#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang.h"

#include <vector>

#include "Logger.h"
#include "IGLIdable.h"
#include "IGLBindable.h"
#include "TextureRender.h"
#include "TextureDepth.h"

class Framebuffer : public IGLBindable, public IGLIdable
{
public:
    enum AttachmentType
    {
        Color = GL_COLOR_ATTACHMENT0, //For position, normal, uv, diffuse, etc.
        Depth = GL_DEPTH_ATTACHMENT   //For Depth
    };

private:

    int width, height;
    GLuint depthBufferAttachmentId;

    std::vector<GLuint> boundAttachments;
    std::vector<TextureRender*> textureAttachments;

protected:

    void CheckFramebufferError() const;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateTextureAttachment(int framebufferAttachmentNum,
                                 AttachmentType attachmentType = AttachmentType::Color);

    void CreateDepthBufferAttachment();

    TextureRender* GetTextureAttachment(int framebufferAttachmentNum) const;

    void SetReadBuffer(int attachmentId) const;
    Vector3 ReadPixel(int x, int y, int attachmentId) const;
    void Resize(int width, int height);

    void Clear() const;
    void ClearDepth() const;
    void ClearColor() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;
};

#endif // FRAMEBUFFER_H
