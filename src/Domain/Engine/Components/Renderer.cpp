#include "Renderer.h"

Renderer::Renderer()
{
}

Material *Renderer::GetMaterial()
{
    return material;
}

void Renderer::ActivateStatesBeforeRendering() const
{
    glLineWidth(lineWidth);
    glEnable(GL_LINE_SMOOTH); //Line antialiasing

    //Set polygon mode
    if(drawWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //Set culling mode
    if(cullMode != CullMode::None)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(cullMode);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::SetDrawWireframe(bool drawWireframe)
{
    this->drawWireframe = drawWireframe;
}
bool Renderer::GetDrawWireframe() const
{
    return drawWireframe;
}


void Renderer::SetCullMode(Renderer::CullMode cullMode)
{
    this->cullMode = cullMode;
}
Renderer::CullMode Renderer::GetCullMode() const
{
    return cullMode;
}


void Renderer::SetRenderMode(Renderer::RenderMode renderMode)
{
    this->renderMode = renderMode;
}
Renderer::RenderMode Renderer::GetRenderMode() const
{
    return renderMode;
}


void Renderer::SetLineWidth(float w)
{
    lineWidth = w;
}
float Renderer::GetLineWidth() const
{
    return lineWidth;
}
