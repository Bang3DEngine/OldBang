#ifndef GBUFFER_H
#define GBUFFER_H

#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

class GBuffer : public Framebuffer
{
public:

    // normal.z doesnt need to be stored since we assume normal is normalized.
    // (normal.x, normal.y, depth_high, depth_low)
    static const GL::Attachment AttNormalDepth = GL::Attachment::Color0;

    // (diffColor.r, diffColor.g, diffColor.b, diffColor.a)
    static const GL::Attachment AttDiffuse     = GL::Attachment::Color1;

    // (receivesLighting, shininess, flags, ---)
    static const GL::Attachment AttMisc        = GL::Attachment::Color2;

    // We need 2 color textures to PingPong them. This is to be able to do
    // screen passes, avoiding writing and reading to the same texture at
    // the same time
    static const GL::Attachment AttColor     = GL::Attachment::Color3;
    static const GL::Attachment AttColorRead = GL::Attachment::Color4;

    static const GL::Attachment AttDepthStencil = GL::Attachment::DepthStencil;

    GBuffer(int width, int height);
    virtual ~GBuffer();

    virtual void PrepareForRender(ShaderProgram *sp);

    void ApplyPass(ShaderProgram *sp,
                   bool willReadFromColor = false,
                   const Rect &mask = Rect::ScreenRect);

    void PrepareColorReadBuffer(const Rect &readNDCRect = Rect::ScreenRect);

    void SetAllColorDrawBuffers() const override;
    /**
     * @brief Sets all GBuffer's draw buffers, except for the color one.
     * This is useful for G render pass.
     */
    void SetAllDrawBuffersExceptColor();
    void SetColorDrawBuffer();

    void ClearDepth(float clearDepth = 1.0f) override;

    void ClearBuffersAndBackground(const Color &backgroundColor);

private:
    RenderTexture *m_normalTexture       = nullptr;
    RenderTexture *m_diffuseTexture      = nullptr;
    RenderTexture *m_miscTexture         = nullptr;
    RenderTexture *m_colorTexture        = nullptr;
    RenderTexture *m_colorReadTexture    = nullptr;
    RenderTexture *m_depthStencilTexture = nullptr;

    mutable bool m_willReadFromColorRead = false;

    void RenderScreenPlane();

    friend class GraphicPipeline;
};

NAMESPACE_BANG_END

#endif // GBUFFER_H
