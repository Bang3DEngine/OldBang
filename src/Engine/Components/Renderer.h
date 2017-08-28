#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>

#include "Bang/GL.h"
#include "Bang/GLObject.h"
#include "Bang/Component.h"
#include "Bang/RenderPass.h"

FORWARD   class Camera;
FORWARD   class Material;
FORWARD   class SceneManager;

class Renderer : public GLObject,
                 public Component
{
    COMPONENT(Renderer)

public:
    virtual void OnRender();

    // GLObject
    virtual void Bind() const override;
    virtual void UnBind() const override;

    // Component
    virtual void OnRender(RenderPass renderPass) override;

    void UseMaterialCopy();

    void SetVisible(bool visible);
    void SetMaterial(Material *m);
    void SetRenderWireframe(bool m_drawWireframe);
    void SetCullMode(GL::CullMode m_cullMode);
    void SetViewProjMode(GL::ViewProjMode viewProjMode);
    void SetRenderPrimitive(GL::Primitives m_renderMode);
    void SetLineWidth(float w);
    void SetRenderPass(RenderPass rp);

    bool GetVisible() const;
    Material* GetMaterial() const;
    Material* GetSharedMaterial() const;
    bool GetRenderWireframe() const;
    GL::CullMode GetCullMode() const;
    GL::ViewProjMode GetViewProjMode() const;
    GL::Primitives GetRenderPrimitive() const;
    float GetLineWidth() const;
    RenderPass GetRenderPass() const;

    // Renderer
    virtual AABox GetAABBox() const;
    virtual Rect GetBoundingRect(Camera *camera = nullptr) const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    bool m_drawWireframe        = false;
    GL::CullMode m_cullMode     = GL::CullMode::Back;
    GL::Primitives m_renderMode = GL::Primitives::Triangles;
    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::UseBoth;

    Renderer();
    virtual ~Renderer();

private:
    bool m_visible = true;
    float m_lineWidth = 1.0f;
    Material *m_material     = nullptr;
    Material *m_materialCopy = nullptr;
    RenderPass m_renderPass = RenderPass::Scene_Lighted;

    friend class GraphicPipeline;
};

#endif // RENDERER_H
