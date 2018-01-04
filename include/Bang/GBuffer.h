#ifndef GBUFFER_H
#define GBUFFER_H

#include "Bang/GL.h"
#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class ShaderProgram;

class GBuffer : public Framebuffer
{
public:

    // (normal.x, normal.y, normal.z, 0)
    static const GL::Attachment AttNormal       = GL::Attachment::Color0;

    // (diffColor.r, diffColor.g, diffColor.b, diffColor.a)
    static const GL::Attachment AttDiffuse      = GL::Attachment::Color1;

    // (receivesLighting, shininess, flags, ---)
    static const GL::Attachment AttMisc         = GL::Attachment::Color2;

    static const GL::Attachment AttColor        = GL::Attachment::Color3;
    static const GL::Attachment AttColorRead    = GL::Attachment::Color4;

    static const GL::Attachment AttDepthStencil = GL::Attachment::DepthStencil;

    GBuffer(int width, int height);
    virtual ~GBuffer();

    virtual void PrepareForRender(ShaderProgram *sp);

    void ApplyPass(ShaderProgram *sp,
                   bool willReadFromColor = false,
                   const Rect &mask = Rect::NDCRect);

    void PrepareColorReadBuffer(const Rect &readNDCRect = Rect::NDCRect);

    void SetAllDrawBuffers() const override;
    void SetAllDrawBuffersExceptColor();
    void SetColorDrawBuffer();

    void ClearAllBuffersExceptColor();
    void ClearBuffersAndBackground(const Color &backgroundColor);

private:
    void RenderScreenPlane();

    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // GBUFFER_H
