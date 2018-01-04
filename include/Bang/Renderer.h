#ifndef RENDERER_H
#define RENDERER_H

#include "Bang/GL.h"
#include "Bang/GLObject.h"
#include "Bang/Component.h"
#include "Bang/RenderPass.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ResourceHandle.h"
#include "Bang/IMaterialChangedListener.h"
#include "Bang/IRendererChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Material;
FORWARD class SceneManager;

class Renderer : public Component,
                 public GLObject,
                 public IMaterialChangedListener,
                 public EventEmitter<IRendererChangedListener>
{
    COMPONENT(Renderer)

public:
    virtual void OnRender();

    // GLObject
    virtual void Bind() const override;
    virtual void UnBind() const override;

    // Component
    virtual void OnRender(RenderPass renderPass) override;

    void SetVisible(bool visible);
    void SetMaterial(Material *mat);
    void SetRenderWireframe(bool renderWireframe);
    void SetCullFace(GL::Face cullMode);
    void SetCulling(bool cull);
    void SetViewProjMode(GL::ViewProjMode viewProjMode);
    void SetRenderPrimitive(GL::Primitive renderPrimitive);
    void SetLineWidth(float w);
    void SetRenderPass(RenderPass rp);

    bool IsVisible() const;
    Material* GetSharedMaterial() const;
    Material* GetUserMaterial() const;
    Material* GetMaterial() const;
    bool IsRenderWireframe() const;
    GL::Face GetCullFace() const;
    bool GetCulling() const;
    GL::ViewProjMode GetViewProjMode() const;
    GL::Primitive GetRenderPrimitive() const;
    float GetLineWidth() const;
    RenderPass GetRenderPass() const;

    // IMaterialChangedListener
    void OnMaterialChanged(const Material *changedMaterial) override;

    // Renderer
    virtual AABox GetAABBox() const;
    virtual Rect GetBoundingRect(Camera *camera) const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    bool m_cullling = true;
    bool m_renderWireframe = false;
    GL::Face m_cullFace = GL::Face::Back;
    GL::Primitive m_renderPrimitive = GL::Primitive::Triangles;
    GL::ViewProjMode m_viewProjMode = GL::ViewProjMode::UseBoth;

    Renderer();
    virtual ~Renderer();

    void PropagateRendererChanged();

private:
    bool m_visible = true;
    mutable RH<Material> p_material;
    RH<Material> p_sharedMaterial;
    float m_lineWidth = 1.0f;
    RenderPass m_renderPass = RenderPass::Scene_Lighted;

    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // RENDERER_H
