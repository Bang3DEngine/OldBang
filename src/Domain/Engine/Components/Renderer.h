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

    /**
     * @brief ignoreModelMatrix
     */
    bool ignoreModelMatrix = false;

    /**
     * @brief If ignoreViewMatrix == true, when drawing the Renderer
     * will ignore view matrix.
     * So it won't take into account camera's translation, rotation or scale.
     */
    bool ignoreViewMatrix = false;


protected:

    Material *material = nullptr;

    bool drawWireframe = false;
    CullMode cullMode = CullMode::Back;
    RenderMode renderMode = RenderMode::Triangles;

    virtual void ActivateStatesBeforeRendering() const;
    virtual void OnRender() override = 0;
    virtual void Render() const = 0;

    void GetMatrices(glm::mat4 &model,
                     glm::mat4 &view,
                     glm::mat4 &projection,
                     glm::mat4 &pvm) const;

    void SetMatricesUniforms(
            const glm::mat4 &model,
            const glm::mat4 &view,
            const glm::mat4 &projection,
            const glm::mat4 &pvm) const;

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

    void SetIgnoreModelMatrix(bool worldCoords);
    bool GetIgnoreModelMatrix() const;

    void SetIgnoreViewMatrix(bool ignore);
    bool GetIgnoreViewMatrix() const;

    virtual const std::string ToString() const override { return "Renderer"; }
    virtual std::string GetName() const override { return "Renderer"; }
};

#endif // RENDERER_H
