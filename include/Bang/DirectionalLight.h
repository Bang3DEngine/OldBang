#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Bang/Light.h"
#include "Bang/Framebuffer.h"

NAMESPACE_BANG_BEGIN

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

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Framebuffer *m_shadowMapFramebuffer = nullptr;

    DirectionalLight();
    virtual ~DirectionalLight();
};

NAMESPACE_BANG_END

#endif // DIRECTIONALLIGHT_H
