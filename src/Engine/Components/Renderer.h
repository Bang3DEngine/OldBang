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
    virtual void CloneInto(ICloneable *clone) const override;

    virtual void SetMaterial(Material *m);
    virtual Material* GetMaterial() const;
    virtual Material* GetSharedMaterial() const;

    void SetRenderWireframe(bool m_drawWireframe);
    bool GetRenderWireframe() const;

    virtual AABox GetAABBox() const;

    virtual Rect GetBoundingRect(Camera *camera = nullptr) const;

    void SetCullMode(GL::CullMode m_cullMode);
    GL::CullMode GetCullMode() const;

    void SetViewProjMode(GL::ViewProjMode viewProjMode);
    GL::ViewProjMode GetViewProjMode() const;

    void SetRenderPrimitive(GL::RenderPrimitive m_renderMode);
    GL::RenderPrimitive GetRenderPrimitive() const;

    void SetLineWidth(float w);
    float GetLineWidth() const;

    virtual void OnRender(RenderPass renderPass) override;
    virtual void OnRender();

    void UseMaterialCopy();

    virtual void Bind() const override;
    virtual void UnBind() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void SetRenderPass(RenderPass rp);
    RenderPass GetRenderPass() const;

protected:
    bool m_drawWireframe        = false;
    GL::CullMode m_cullMode     = GL::CullMode::Back;
    GL::RenderPrimitive m_renderMode = GL::RenderPrimitive::Triangles;
    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::UseBoth;

    Renderer();
    virtual ~Renderer();

private:
    float m_lineWidth = 1.0f;
    Material *m_material     = nullptr;
    Material *m_materialCopy = nullptr;
    RenderPass m_renderPass = RenderPass::Scene_Lighted;

    friend class GraphicPipeline;
};

#endif // RENDERER_H
