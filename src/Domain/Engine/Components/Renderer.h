#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "Rect.h"
#include "Array.h"
#include "NamedEnum.h"
#include "Component.h"

class AABox;
class Camera;
class Material;
class SceneManager;
class SelectionFramebuffer;
class Renderer : public Component
{
public:
    NamedEnum (RenderMode,
        Points = GL_POINTS,
        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        Triangles = GL_TRIANGLES,
        Quads = GL_QUADS
    );

    NamedEnum (CullMode,
        Front = GL_FRONT,
        Back = GL_BACK,
        FrontAndBack = GL_FRONT_AND_BACK,
        None = GL_NONE
    );

    enum DepthLayer
    {
        DepthLayerScene,
        DepthLayerCanvas,
        DepthLayerGizmos
    };

private:

    DepthLayer m_depthLayer = DepthLayerScene;
    bool m_isGizmo = false;
    bool m_isTransparent = false;

    /**
     * @brief Width of the lines if rendering with Lines RenderMode
     * or if rendering with wireframe.
     */
    float m_lineWidth = 1.0f;

    #ifdef BANG_EDITOR
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
    #endif

protected:

    Material *m_material = nullptr;

    bool m_drawWireframe = false;
    bool m_receivesLighting = true;
    bool m_hasCustomPRPass = false;
    CullMode m_cullMode = CullMode::Back;
    RenderMode m_renderMode = RenderMode::Triangles;

    virtual void ActivateGLStatesBeforeRendering(Material *mat) const;
    virtual void RenderWithoutBindingMaterial() const = 0;
    virtual void RenderForSelectionFramebufferWithoutBindingMaterial() const;

    void GetMatrices(Matrix4 *model,
                     Matrix4 *normal,
                     Matrix4 *view,
                     Matrix4 *projection,
                     Matrix4 *pvm) const;

    virtual void SetMatricesUniforms(
            Material *mat,
            const Matrix4 &model,
            const Matrix4 &normal,
            const Matrix4 &view,
            const Matrix4 &projection,
            const Matrix4 &pvm) const;

protected:
    Renderer();

public:

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMaterial(Material *m) = 0;
    virtual Material* GetMaterial() const;

    void SetDrawWireframe(bool m_drawWireframe);
    bool GetDrawWireframe() const;

    virtual AABox GetAABBox() const = 0;

    /**
     * @brief GetBoundingRect
     * @param camera The camera whose screen space you want the bounding rect
     * to be relative to. By default, it will take the active scene's camera.
     * @return Returns the bounding rect of the renderer relative to the
     * passed camera's screen space
     */
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const;

    void SetCullMode(CullMode m_cullMode);
    CullMode GetCullMode() const;

    void SetRenderMode(RenderMode m_renderMode);
    RenderMode GetRenderMode() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    void SetReceivesLighting(bool receivesLighting);
    bool ReceivesLighting() const;

    virtual void Render() const;
    virtual void RenderCustomPR() const;
    virtual void RenderWithMaterial(Material *mat) const;

    virtual bool IsACanvasRenderer() const;

    #ifdef BANG_EDITOR
    void SetActivateGLStatesBeforeRenderingForSelectionFunction(const std::function<void()> &f);

    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual const String ToString() const override { return "Renderer"; }
    virtual String GetName() const override { return "Renderer"; }
    virtual ICloneable *Clone() const override = 0;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void SetTransparent(bool transparent);
    bool IsTransparent() const;

    void SetIsGizmo(bool isGizmo);
    bool IsGizmo() const;

    bool HasCustomPRPass() const;

    void SetDepthLayer(DepthLayer dl);
    DepthLayer GetDepthLayer() const;

    friend class GameObject;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // RENDERER_H
