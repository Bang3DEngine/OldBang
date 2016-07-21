#ifndef GBUFFER_H
#define GBUFFER_H

#include "Bang.h"

#include "Mesh.h"
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
        MaterialBools, // (receivesLighting, -, -, -)
        MaterialLightProperties, // ()
        Depth,
        Color
    };

private:

    Material *p_renderGBufferToScreenMaterial;
    Mesh *p_planeMeshToRenderEntireScreen;

    void RenderScreenPlane() const;

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void BindInTexturesTo(Material *mat) const;
    void RenderPassWithMaterial(Material *mat) const;
    void RenderToScreen() const;
};

#endif // GBUFFER_H
