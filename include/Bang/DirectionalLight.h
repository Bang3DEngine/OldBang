#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Texture2D;

class DirectionalLight : public Light
{
    COMPONENT(DirectionalLight)

public:
    Texture2D* GetShadowMap() const;

    // Component
    void OnRender(RenderPass rp) override;

    // Light
    void RenderShadowMaps() override;
    void SetUniformsBeforeApplyingLight(Material* mat) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Framebuffer *m_shadowMapFramebuffer = nullptr;

    DirectionalLight();
    virtual ~DirectionalLight();

    void GetShadowMapMatrices(Scene *scene,
                              Matrix4 *viewMatrix,
                              Matrix4 *projMatrix) const;
    Matrix4 GetShadowMapMatrix(Scene *scene) const;
    Matrix4 GetLightDirMatrix() const;
    AABox GetShadowMapOrthoBox(Scene *scene) const;
};

NAMESPACE_BANG_END

#endif // DIRECTIONALLIGHT_H
