#ifndef GBUFFER_H
#define GBUFFER_H

#include "Bang.h"

#include "Mesh.h"
#include "Material.h"
#include "MeshFactory.h"
#include "Framebuffer.h"

#include "Color.h"

class GBuffer : public Framebuffer
{
public:
    enum Attachment
    {
        Position = 0,
        Normal,
        Uv,
        Diffuse,
        MaterialBools, // (receivesLighting, -, -, -)
        Depth,
        Color
    };

private:

    Material *m_renderGBufferToScreenMaterial = nullptr;
    Mesh *m_planeMeshToRenderEntireScreen = nullptr;

    void RenderScreenPlane() const;

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void BindGBufferInTexturesTo(Material *mat) const;
    void RenderPassWithMaterial(Material *mat) const;
    void RenderToScreen() const;

    void ClearBuffersAndBackground(const ::Color& backgroundColor, const ::Color& clearValue = ::Color::zero);
};

#endif // GBUFFER_H
