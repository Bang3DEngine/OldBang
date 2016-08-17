#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang.h"

#include <vector>

#include "Color.h"
#include "Logger.h"
#include "IGLIdable.h"
#include "IGLBindable.h"
#include "TextureRender.h"
#include "TextureDepth.h"

class Framebuffer : public IGLBindable
                   ,public IGLIdable
{

private:

    int m_width, m_height;
    GLuint m_depthBufferAttachmentId;

    std::vector<GLuint> m_boundAttachments;
    std::vector<TextureRender*> m_textureAttachments;

protected:

    void CheckFramebufferError() const;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateColorAttachment(int framebufferAttachmentNum,
                               GLint glInternalFormat = -1, GLint glFormat = -1,  GLint glInternalType = -1,
                               bool depthAttachment = false);

    void CreateDepthBufferAttachment();

    TextureRender* GetTextureAttachment(int framebufferAttachmentNum) const;

    void SetAllDrawBuffers() const;
    void SetDrawBuffers(const std::vector<int> &attachmentIds) const;
    void SetReadBuffer(GLuint attachmentId) const;

    Color ReadColor255(int x, int y, int attachmentId,
                       int glFormat = GL_RGBA, int glType = GL_UNSIGNED_BYTE) const;
    Color ReadColor(int x, int y, int attachmentId,
                    int glFormat = GL_RGBA, int glType = GL_UNSIGNED_BYTE) const;
    float ReadFloat(int x, int y, int attachmentId,
                    int glFormat = GL_DEPTH_COMPONENT, int glType = GL_FLOAT) const;
    float ReadDepth(int x, int y) const; // TODO: not working read from renderbuffer
    void Resize(int width, int height);

    void Clear() const;
    void ClearDepth() const;
    void ClearColor() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;
};

#endif // FRAMEBUFFER_H
