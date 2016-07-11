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
        Depth
    };

private:

    Material *p_renderToScreenMaterial;
    Mesh *p_renderToScreenPlaneMesh;

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void RenderToScreen() const;
};

#endif // GBUFFER_H
