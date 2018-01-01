#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang/Map.h"
#include "Bang/Array.h"
#include "Bang/Color.h"
#include "Bang/GLObject.h"
#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

class Framebuffer : public GLObject
{
public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void CreateAttachment(GL_Attachment attachment, GL_ColorFormat texFormat);

    virtual void SetAllDrawBuffers() const;
    void SetDrawBuffers(const Array<GL_Attachment> &attachments) const;
    void SetReadBuffer(GL_Attachment attachment) const;
    void SetAttachmentTexture(Texture2D *tex, GL_Attachment attachment);

    Color ReadColor(int x, int y, GL_Attachment attachment) const;
    void Resize(int width, int height);

    int GetWidth() const;
    int GetHeight() const;
    Vector2 GetSize() const;
    GL_Attachment GetCurrentReadAttachment() const;
    const Array<GL_Attachment>& GetCurrentDrawAttachments() const;
    Texture2D* GetAttachmentTexture(GL_Attachment attachment) const;
    GL_BindTarget GetGLBindTarget() const override;

    void Clear();
    virtual void ClearDepth(float clearDepth = 1.0f);
    void ClearColor(const Color &clearColor = Color::Zero);

    virtual void Bind() const override;
    virtual void UnBind() const override;

    void Export(GL_Attachment attachmentId, const Path &filepath,
                bool invertY = true) const;
    void ExportStencil(const Path &filepath,
                       int stencilValueMultiplier = 1) const;

protected:
    int m_width = 0;
    int m_height = 0;
    Array<GL_Attachment> m_attachments;
    Map<GL_Attachment, RH<Texture2D>> m_attachments_To_Texture;

    void PushDrawAttachments();
    void PopDrawAttachments();

private:
    mutable Array<GL_Attachment> m_currentDrawAttachments;
    mutable Array<GL_Attachment> m_latestDrawAttachments;
    mutable GL_Attachment m_currentReadAttachment;
};

NAMESPACE_BANG_END

#endif // FRAMEBUFFER_H
