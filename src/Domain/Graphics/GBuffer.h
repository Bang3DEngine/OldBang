#ifndef GBUFFER_H
#define GBUFFER_H

#include <stack>

#include "Bang.h"

#include "Mesh.h"
#include "Color.h"
#include "Material.h"
#include "MeshFactory.h"
#include "Framebuffer.h"


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

    TextureRender *m_positionTexture = nullptr;
    TextureRender *m_normalTexture   = nullptr;
    TextureRender *m_uvTexture       = nullptr;
    TextureRender *m_diffuseTexture  = nullptr;
    TextureRender *m_matPropsTexture = nullptr;
    TextureRender *m_depthTexture    = nullptr;
    TextureRender *m_stencilTexture  = nullptr;
    TextureRender *m_colorTexture    = nullptr;

private:

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_planeMeshToRenderEntireScreen = nullptr;

    void RenderScreenPlane() const;

    // To preserve state
    mutable Array<int> m_previousDrawAttachmentsIds;
    void SaveCurrentDrawBuffers() const;
    void LoadSavedDrawBuffers() const;

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void BindInputTexturesTo(Material *mat) const;
    void RenderPassWithMaterial(Material *mat) const;
    void RenderToScreen(Attachment attachmentId) const;
    void RenderToScreen() const;

    /**
     * @brief Sets all GBuffer's draw buffers, except for the color one.
     * This is useful for D2G render pass.
     */
    void SetAllDrawBuffersExceptColor() const;
    void SetStencilDrawBuffer() const;
    void SetColorDrawBuffer() const;

    void ClearStencil() const;
    void ClearAllBuffersExceptColor();
    void ClearBuffersAndBackground(const ::Color& backgroundColor, const ::Color& clearValue = ::Color::zero);
};

#endif // GBUFFER_H
