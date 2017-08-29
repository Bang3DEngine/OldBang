#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang/Map.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/GLObject.h"
#include "Bang/Texture2D.h"

FORWARD class G_RenderTexture;

class G_Framebuffer : public GLObject
{
public:
    G_Framebuffer(int width, int height);
    virtual ~G_Framebuffer();

    void CreateColorAttachment(GL::Attachment attachment,
                               GL::ColorFormat texFormat);
    void CreateDepthRenderbufferAttachment();

    G_RenderTexture* GetAttachmentTexture(GL::Attachment attachment) const;

    virtual void SetAllDrawBuffers() const;
    void SetDrawBuffers(const Array<GL::Attachment> &attachments) const;
    void SetReadBuffer(GL::Attachment attachment) const;

    const Array<GL::Attachment>& GetCurrentDrawAttachmentIds() const;

    Color ReadColor(int x, int y, GL::Attachment attachment) const;
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

    void SaveToImage(GL::Attachment attachmentId, const Path &filepath,
                     bool invertY = true) const;
    void SaveStencilToImage(const Path &filepath,
                            int stencilValueMultiplier = 1) const;

protected:
    int m_width = 0;
    int m_height = 0;
    GLuint m_depthRenderBufferId = 0;
    Array<GL::Attachment> m_colorAttachmentIds;
    Map<GL::Attachment, G_RenderTexture*> m_attachmentId_To_Texture;

    void PushDrawAttachmentIds();
    void PopDrawAttachmentIds();

private:
    mutable Array<GL::Attachment> m_currentDrawAttachmentIds;
    mutable Array<GL::Attachment> m_latestDrawAttachmentIds;
};

#endif // FRAMEBUFFER_H
