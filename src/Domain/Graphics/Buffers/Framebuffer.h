#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Array.h"

#include "Map.h"
#include "Color.h"
#include "GLObject.h"

class RenderTexture;
class Framebuffer : public GLObject
{
public:
    enum AttachmentId
    {
        ColorAttachment0 = GL_COLOR_ATTACHMENT0,
        ColorAttachment1 = GL_COLOR_ATTACHMENT1,
        ColorAttachment2 = GL_COLOR_ATTACHMENT2,
        ColorAttachment3 = GL_COLOR_ATTACHMENT3,
        ColorAttachment4 = GL_COLOR_ATTACHMENT4,
        ColorAttachment5 = GL_COLOR_ATTACHMENT5,
        ColorAttachment6 = GL_COLOR_ATTACHMENT6,
        ColorAttachment7 = GL_COLOR_ATTACHMENT7,
        DepthAttachment  = GL_DEPTH_ATTACHMENT
    };

    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateColorAttachment(AttachmentId attId);
    void CreateDepthRenderbufferAttachment();

    RenderTexture* GetAttachmentTexture(AttachmentId attId) const;

    virtual void SetAllDrawBuffers() const;
    void SetDrawBuffers(const Array<AttachmentId> &attIds) const;
    void SetReadBuffer(AttachmentId attId) const;

    const Array<AttachmentId>& GetCurrentDrawAttachmentIds() const;

    Color ReadColor(int x, int y, AttachmentId attId) const;
    void Resize(int width, int height);

    int GetWidth() const;
    int GetHeight() const;
    Vector2 GetSize() const;

    void Clear();
    virtual void ClearDepth(float clearDepth = 1.0f);
    void ClearColor(const Color &clearColor = Color::Zero);

    GL::BindTarget GetGLBindTarget() const override;
    virtual void Bind() const override;
    virtual void UnBind() const override;

    void SaveToImage(AttachmentId attachmentId, const String &filepath,
                     bool invertY = false) const;

protected:
    int m_width = 0;
    int m_height = 0;
    GLuint m_depthRenderBufferId = 0;
    Array<AttachmentId> m_colorAttachmentIds;
    Map<AttachmentId, RenderTexture*> m_attachmentId_To_Texture;
};

#endif // FRAMEBUFFER_H
