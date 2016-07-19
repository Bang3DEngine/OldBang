#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <functional>

#include "Box.h"
#include "Material.h"
#include "Component.h"

class Renderer : public Component
{
friend class SelectionFramebuffer;

public:
    enum RenderMode
    {
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
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
    float m_lineWidth = 1.0f;

    /**
     * @brief ignoreModelMatrix
     */
    bool m_ignoreModelMatrix = false;

    /**
     * @brief If ignoreViewMatrix == true, when drawing the Renderer
     * will ignore view matrix.
     * So it won't take into account camera's translation, rotation or scale.
     */
    bool m_ignoreViewMatrix = false;

    /**
     * @brief If ignoreViewMatrix == true, when drawing the Renderer
     * will ignore view matrix.
     * So it won't take into account camera's translation, rotation or scale.
     */
    bool m_ignoreProjectionMatrix = false;

    /**
     * @brief Called by SelectionFramebuffer.
     * Use this if you want to activate
     * special states before rendering for the selection framebuffer. For example,
     * you may want to draw a bigger object in order for the selection to be easier to do.
     *
     * This is used in the transform axes for example, in which we increase the lineWidth for
     * an easier axis grabbing.
     */
    std::function<void()> ActivateGLStatesBeforeRenderingForSelection = nullptr;

protected:

    Material *p_material = nullptr;

    bool m_drawWireframe = false;
    bool m_receivesLighting = true;
    CullMode m_cullMode = CullMode::Back;
    RenderMode m_renderMode = RenderMode::Triangles;


    virtual void ActivateGLStatesBeforeRendering() const;
    virtual void RenderWithoutBindingMaterial() const = 0;
    void OnRender() override;
    void Render() const;

    void GetMatrices(Matrix4 &model,
                     Matrix4 &view,
                     Matrix4 &projection,
                     Matrix4 &pvm) const;

    void SetMatricesUniforms(
            const Matrix4 &model,
            const Matrix4 &view,
            const Matrix4 &projection,
            const Matrix4 &pvm) const;

public:

    Renderer();
    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMaterial(Material *m) = 0;
    virtual Material* GetMaterial();

    void SetDrawWireframe(bool m_drawWireframe);
    bool GetDrawWireframe() const;

    virtual Box GetBoundingBox() const = 0;

    void SetCullMode(CullMode m_cullMode);
    CullMode GetCullMode() const;

    void SetRenderMode(RenderMode m_renderMode);
    RenderMode GetRenderMode() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    void SetIgnoreModelMatrix(bool worldCoords);
    bool GetIgnoreModelMatrix() const;

    void SetIgnoreViewMatrix(bool ignore);
    bool GetIgnoreViewMatrix() const;

    void SetIgnoreProjectionMatrix(bool ignore);
    bool GetIgnoreProjectionMatrix() const;

    void SetReceivesLighting(bool receivesLighting);
    bool GetReceivesLighting() const;

    void SetActivateGLStatesBeforeRenderingForSelectionFunction(const std::function<void()> &f);

    virtual const std::string ToString() const override { return "Renderer"; }
    virtual std::string GetName() const override { return "Renderer"; }
    virtual ICloneable *Clone() const override = 0;
};

#endif // RENDERER_H
