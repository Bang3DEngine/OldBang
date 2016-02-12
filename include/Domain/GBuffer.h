#ifndef GBUFFER_H
#define GBUFFER_H

#include "Material.h"
#include "MeshScreenPlane.h"
#include "Framebuffer.h"

class GBuffer : public Framebuffer
{
public:
    enum Attachment
    {
        Position = 0,
        Normal,
        Uv,
        Diffuse
    };

private:
    //Simple material with pass by shaders
    //When you want to render with it, just attach
    //the final color texture to texture slot 0
    Material *renderToScreenMaterial;
    MeshScreenPlane *screenPlaneMesh; //This will be used to render to screen

public:
    GBuffer(int width, int height);
    virtual ~GBuffer();

    void RenderToScreen() const;
};

#endif // GBUFFER_H
