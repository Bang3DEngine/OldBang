#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Bang.h"

#include <vector>

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

    int m_width, m_height;
    GLuint m_depthAttachmentId;

    std::map<int, GLuint> m_attachmentId_To_GLAttachment;
    std::map<int, TextureRender*> m_attachmentId_To_Texture;

protected:

    void CheckFramebufferError() const;

public:
    Framebuffer(int width, int height);
    virtual ~Framebuffer();

    void SetColorAttachment(int attachmentId, TextureRender *tex);
    void CreateDepthRenderbufferAttachment();

    TextureRender* GetColorAttachment(int attachmentId) const;

    void SetAllDrawBuffers() const;
    void SetDrawBuffers(const std::vector<int> &attachmentIds) const;
    void SetReadBuffer(int attachmentId) const;

    Color ReadColor(int x, int y, int attachmentId) const;
    float ReadDepth(int x, int y) const; // TODO: not working read from renderbuffer
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
