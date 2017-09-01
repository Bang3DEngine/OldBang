#ifndef GBUFFER_H
#define GBUFFER_H

#include "Bang/Rect.h"
#include "Bang/Color.h"
#include "Bang/G_Framebuffer.h"

class G_GBuffer : public G_Framebuffer
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

    G_GBuffer(int width, int height);
    virtual ~G_GBuffer();

    virtual void BindTextureBuffersTo(G_ShaderProgram *sp,
                                      bool willReadFromColor = false) const;

    void ApplyPass(G_ShaderProgram *sp,
                   bool prepareReadFromColorBuffer = false,
                   const Rect &mask = Rect::ScreenRect);

    void PrepareColorReadBuffer(const Rect &readNDCRect = Rect::ScreenRect);

    void SetAllDrawBuffers() const override;
    /**
     * @brief Sets all G_GBuffer's draw buffers, except for the color one.
     * This is useful for G render pass.
     */
    void SetAllDrawBuffersExceptColor();
    void SetColorDrawBuffer();

    void ClearDepth(float clearDepth = 1.0f) override;

    void ClearBuffersAndBackground(const Color &backgroundColor);

private:
    G_RenderTexture *m_normalTexture    = nullptr;
    G_RenderTexture *m_diffuseTexture   = nullptr;
    G_RenderTexture *m_miscTexture      = nullptr;
    G_RenderTexture *m_colorTexture     = nullptr;
    G_RenderTexture *m_colorReadTexture = nullptr;

    mutable bool m_willReadFromColorRead = false;

    void RenderScreenPlane();

    friend class GraphicPipeline;
};

#endif // GBUFFER_H
