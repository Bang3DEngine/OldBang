#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <functional>

#include "GL.h"
#include "Rect.h"
#include "Array.h"
#include "NamedEnum.h"
#include "Component.h"
#include "IRenderAgent.h"

class AABox;
class Camera;
class Material;
class SceneManager;
class SelectionFramebuffer;
class Renderer : public Component,
                 public IRenderAgent
{
public:
    enum DepthLayer
    {
        DepthLayerScene,
        DepthLayerCanvas,
        DepthLayerGizmos
    };

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMaterial(Material *m);
    virtual Material* GetMaterial() const;
    virtual Material* GetSharedMaterial() const;

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

    void SetCullMode(GL::CullMode m_cullMode);
    GL::CullMode GetCullMode() const;

    void SetRenderMode(GL::RenderMode m_renderMode);
    GL::RenderMode GetRenderMode() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    virtual void Render() const;
    virtual void RenderWithMaterial(Material *mat) const;

    void UseMaterialCopy();

    #ifdef BANG_EDITOR
    void SetOnRenderingStartsForSelectionFunction(
            const std::function<void()> &f);
    #endif

    void OnRenderingStarts(GameObject *go, ShaderProgram *sp) const;
    void OnRenderingEnds(GameObject *go, ShaderProgram *sp) const;

    virtual String GetName() const override;
    virtual ICloneable *Clone() const override = 0;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    void SetTransparent(bool transparent);
    bool IsTransparent() const;

    void SetDepthLayer(DepthLayer dl);
    DepthLayer GetDepthLayer() const;

protected:

    bool m_drawWireframe = false;
    Material *m_material = nullptr;
    Material *m_materialCopy = nullptr;
    GL::CullMode m_cullMode     = GL::CullMode::Back;
    GL::RenderMode m_renderMode = GL::RenderMode::Triangles;

    virtual void RenderWithoutMaterial() const = 0;
    virtual void RenderForSelectionWithoutMaterial() const;

    Renderer();
    virtual ~Renderer();

private:

    DepthLayer m_depthLayer = DepthLayerScene;
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
     * special states before rendering for the selection framebuffer.
     * For example, you may want to draw a bigger object in order for
     * the selection to be easier to do.
     *
     * This is used in the transform axes for example, in which we
     * increase the lineWidth for an easier axis grabbing.
     */
    std::function<void()> p_OnRenderingStartsForSelectionFunc = nullptr;
    #endif

    friend class GameObject;
    friend class GraphicPipeline;
    friend class SelectionFramebuffer;
};

#endif // RENDERER_H
