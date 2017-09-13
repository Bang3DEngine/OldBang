#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

FORWARD class GBuffer;
FORWARD class SelectionFramebuffer;

class RenderSurface
{
public:
    RenderSurface();
    RenderSurface(int width, int height);
    virtual ~RenderSurface();

    void Resize(int width, int height);

    GBuffer *GetGBuffer() const;
    SelectionFramebuffer *GetSelectionFramebuffer() const;

private:
    GBuffer *m_gbuffer = nullptr;
    SelectionFramebuffer *m_selectionBuffer = nullptr;
};

NAMESPACE_BANG_END

#endif // RENDERTEXTURE_H
