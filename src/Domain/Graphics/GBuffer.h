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

    void BindGBufferInTexturesTo(Material *mat) const;
    void RenderPassWithMaterial(Material *mat) const;
    void RenderToScreen() const;

    void ClearBuffersAndBackground(const glm::vec4 &backgroundColor, const glm::vec4 &clearValue = glm::vec4(0.0f));
};

#endif // GBUFFER_H
