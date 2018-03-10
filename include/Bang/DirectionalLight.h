#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"
#include "Bang/Matrix4.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Texture2D;

class DirectionalLight : public Light
{
    COMPONENT(DirectionalLight)

public:
    void SetShadowDistance(float shadowDistance);

    float GetShadowDistance() const;
    Texture2D* GetShadowMap() const;

    // Component
    void OnRender(RenderPass rp) override;

    // Light
    void SetUniformsBeforeApplyingLight(Material* mat) const override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Framebuffer *m_shadowMapFramebuffer = nullptr;
    Matrix4 m_lastUsedShadowMapViewProj = Matrix4::Identity;
    float m_shadowDistance = 5.0f;

    DirectionalLight();
    virtual ~DirectionalLight();

    // Light
    void RenderShadowMaps_() override;

    void GetShadowMapMatrices(Scene *scene,
                              Matrix4 *viewMatrix,
                              Matrix4 *projMatrix) const;
    Matrix4 GetShadowMapMatrix(Scene *scene) const;
    Matrix4 GetLightDirMatrix() const;
    AABox GetShadowMapOrthoBox(Scene *scene) const;
};

NAMESPACE_BANG_END

#endif // DIRECTIONALLIGHT_H
