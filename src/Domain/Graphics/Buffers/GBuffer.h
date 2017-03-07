#ifndef GBUFFER_H
#define GBUFFER_H

#include "Rect.h"
#include "Color.h"
#include "String.h"
#include "Framebuffer.h"
#include "IRenderAgent.h"

class Mesh;
class Material;
class GBuffer : public Framebuffer,
                public IRenderAgent
{
public:
    static const AttachmentId AttNormal    = AttachmentId::ColorAttachment0;
    static const AttachmentId AttDiffuse   = AttachmentId::ColorAttachment1;

    // (receivesLighting, shininess, depth, stencil)
    static const AttachmentId AttMisc      = AttachmentId::ColorAttachment2;

    // We need 2 color textures to PingPong them. This is to be able to do
    // screen passes, avoiding writing and reading to the same texture at
    // the same time
    static const AttachmentId AttColor     = AttachmentId::ColorAttachment3;
    static const AttachmentId AttColorRead = AttachmentId::ColorAttachment4;

    GBuffer(int width, int height);
    virtual ~GBuffer();

    virtual void OnRenderingStarts(GameObject *go, ShaderProgram *sp) override;
    virtual void OnRenderingEnds(GameObject *go, ShaderProgram *sp) override;
    void ApplyPass(ShaderProgram *sp,
                   bool copyColorBuffer = false,
                   const Rect &mask = Rect::ScreenRect);
    void RenderToScreen(AttachmentId attachmentId);
    void RenderToScreen();

    void SetAllDrawBuffers() const override;
    /**
     * @brief Sets all GBuffer's draw buffers, except for the color one.
     * This is useful for G render pass.
     */
    void SetAllDrawBuffersExceptColor();
    void SetColorDrawBuffer();

    void SetStencilWrite(bool writeEnabled);
    void SetStencilTest(bool testEnabled);
    void ClearStencil();
    void ClearDepth(float clearDepth = 1.0f) override;
    void ClearStencilDepth();

    void ClearBuffersAndBackground(const ::Color& backgroundColor,
                                   const ::Color& clearValue = ::Color::Zero);

private:
    RenderTexture *m_normalTexture    = nullptr;
    RenderTexture *m_diffuseTexture   = nullptr;
    RenderTexture *m_miscTexture      = nullptr;
    RenderTexture *m_colorTexture     = nullptr;
    RenderTexture *m_colorReadTexture = nullptr;

    bool m_stencilWriteEnabled = false;
    bool m_stencilTestEnabled  = false;

    void RenderScreenPlane();

    friend class GraphicPipeline;
};

#endif // GBUFFER_H
