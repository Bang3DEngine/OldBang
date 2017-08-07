#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "Bang/GL.h"
#include "Bang/GLObject.h"
#include "Bang/Component.h"

FORWARD   class Camera;
FORWARD   class Material;
FORWARD   class SceneManager;

class Renderer : public GLObject, public Component
{
    COMPONENT(Renderer)

public:
    enum RenderLayer
    {
        RLScene, RLCanvas, RLGizmos
    };

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMaterial(Material *m);
    virtual Material* GetMaterial() const;
    virtual Material* GetSharedMaterial() const;

    void SetRenderWireframe(bool m_drawWireframe);
    bool GetRenderWireframe() const;

    virtual AABox GetAABBox() const;

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

    void SetViewProjMode(GL::ViewProjMode viewProjMode);
    GL::ViewProjMode GetViewProjMode() const;

    void SetRenderMode(GL::RenderMode m_renderMode);
    GL::RenderMode GetRenderMode() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    virtual void Render() const;

    void UseMaterialCopy();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetRenderLayer(RenderLayer dl);
    RenderLayer GetRenderLayer() const;

protected:
    bool m_drawWireframe        = false;
    GL::CullMode m_cullMode     = GL::CullMode::Back;
    GL::RenderMode m_renderMode = GL::RenderMode::Triangles;
    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::UseBoth;

    Renderer();
    virtual ~Renderer();

private:
    Material *m_material     = nullptr;
    Material *m_materialCopy = nullptr;

    RenderLayer m_renderLayer = RLScene;

    /**
     * @brief Width of the lines if rendering with Lines RenderMode
     * or if rendering with wireframe.
     */
    float m_lineWidth = 1.0f;

    friend class GraphicPipeline;
};

#endif // RENDERER_H
