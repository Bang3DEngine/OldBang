#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang.h"

#include "Array.h"

#include "Map.h"
#include "Color.h"
#include "Debug.h"
#include "IGLIdable.h"
#include "IGLBindable.h"
#include "TextureDepth.h"
#include "TextureRender.h"

class Framebuffer : public IGLBindable
                   ,public IGLIdable
{

private:

    int m_width;
    int m_height;
    GLuint m_depthAttachmentId = 0;
    Map<int, GLuint> m_attachmentId_To_GLAttachment;
    Map<int, TextureRender*> m_attachmentId_To_Texture;
    mutable Array<int> m_currentDrawAttachmentIds;

protected:

    void CheckFramebufferError() const;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void SetColorAttachment(int attachmentId, TextureRender *tex);
    void CreateDepthRenderbufferAttachment();

    TextureRender* GetColorAttachment(int attachmentId) const;

    void SetAllDrawBuffers() const;
    void SetDrawBuffers(const Array<int> &attachmentIds) const;
    void SetReadBuffer(int attachmentId) const;

    const Array<int>& GetCurrentDrawAttachmentIds() const;

    Color ReadColor(int x, int y, int attachmentId) const;
    void Resize(int width, int height);

    int GetWidth() const;
    int GetHeight() const;
    Vector2 GetSize() const;

    void Clear() const;
    void ClearDepth() const;
    void ClearColor() const;

    virtual void Bind() const override;
    virtual void UnBind() const override;
};

#endif // FRAMEBUFFER_H
