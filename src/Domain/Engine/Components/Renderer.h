#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Material.h"
#include "Component.h"

class Renderer : public Component
{
public:
    enum RenderMode
    {
        Points = GL_POINTS,
        Lines = GL_LINES,
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    };

    enum CullMode
    {
        Front = GL_FRONT,
        Back = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK,
        None = GL_NONE
    };

private:
    /**
     * @brief Width of the lines if rendering with Lines RenderMode
     * or if rendering with wireframe.
     */
    float lineWidth = 1.0f;

protected:

    Material *material = nullptr;

    bool drawWireframe = false;
    CullMode cullMode = CullMode::Back;
    RenderMode renderMode = RenderMode::Triangles;

    virtual void ActivateStatesBeforeRendering() const;
    virtual void OnRender() override = 0;
    virtual void Render() const = 0;

public:
    Renderer();

    virtual void SetMaterial(Material *m) = 0;
    virtual Material* GetMaterial();

    void SetDrawWireframe(bool drawWireframe);
    bool GetDrawWireframe() const;

    void SetCullMode(CullMode cullMode);
    CullMode GetCullMode() const;

    void SetRenderMode(RenderMode renderMode);
    RenderMode GetRenderMode() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    virtual const std::string ToString() const override { return "Renderer"; }
    virtual std::string GetName() const override { return "Renderer"; }
};

#endif // RENDERER_H
