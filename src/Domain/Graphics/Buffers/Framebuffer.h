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
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void SetAttachment(int attachmentId, RenderTexture *tex);
    void CreateDepthRenderbufferAttachment();

    RenderTexture* GetColorAttachment(int attachmentId) const;

    void SetAllDrawBuffers() const;
    void SetDrawBuffers(const Array<int> &attachmentIds) const;
    void SetReadBuffer(int attachmentId) const;

    const Array<int>& GetCurrentDrawAttachmentIds() const;

    Color ReadColor(int x, int y, int attachmentId) const;
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

    void SaveToImage(int attachmentId, const String &filepath,
                     bool invertY = false) const;

private:
    int m_width;
    int m_height;
    GLuint m_depthAttachmentId = 0;
    Map<int, GLuint> m_attachmentId_To_GLAttachment;
    Map<int, RenderTexture*> m_attachmentId_To_Texture;
    mutable Array<int> m_currentDrawAttachmentIds;
};

#endif // FRAMEBUFFER_H
