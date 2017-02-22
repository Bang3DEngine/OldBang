#ifndef GBUFFER_H
#define GBUFFER_H

#include "Rect.h"
#include "Color.h"
#include "String.h"
#include "Framebuffer.h"

class Mesh;
class Material;
class GBuffer : public Framebuffer
{
public:
    enum Attachment
    {
        Position = 0,
        Normal,
        Uv,
        Diffuse,
        MaterialProperties, // (receivesLighting, shininess, -, -)
        Depth,
        Stencil,
        Color
    };

    RenderTexture *m_positionTexture = nullptr;
    RenderTexture *m_normalTexture   = nullptr;
    RenderTexture *m_uvTexture       = nullptr;
    RenderTexture *m_diffuseTexture  = nullptr;
    RenderTexture *m_matPropsTexture = nullptr;
    RenderTexture *m_depthTexture    = nullptr;
    RenderTexture *m_stencilTexture  = nullptr;
    RenderTexture *m_colorTexture    = nullptr;

private:

    bool m_stencilWriteEnabled = false;
    bool m_stencilTestEnabled  = false;

    void RenderScreenPlane();

    // To preserve state
    mutable Array<int> m_previousDrawAttachmentsIds;
    void SaveCurrentDrawBuffers();
    void LoadSavedDrawBuffers();

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void SetUniformsBeforeRendering(Material *mat);
    void RenderPassWithMaterial(Material *mat,
                                const Rect &renderRect = Rect(-1, 1, -1, 1));
    void RenderToScreen(Attachment attachmentId);
    void RenderToScreen();

    /**
     * @brief Sets all GBuffer's draw buffers, except for the color one.
     * This is useful for G render pass.
     */
    void SetAllDrawBuffersExceptColor();
    void SetColorDrawBuffer();

    void SetStencilWrite(bool writeEnabled);
    void SetStencilTest(bool testEnabled);
    void ClearStencil();
    void ClearDepth() override;

    void ClearAllBuffersExceptColor();
    void ClearBuffersAndBackground(const ::Color& backgroundColor, const ::Color& clearValue = ::Color::Zero);
};

#endif // GBUFFER_H
