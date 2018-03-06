#ifndef LIGHT_H
#define LIGHT_H

#include "Bang/Color.h"
#include "Bang/Component.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Material;
FORWARD class Renderer;
FORWARD class GBuffer;

class Light : public Component
{
public:
    enum class ShadowType { NONE = 0, HARD, SOFT };

    void SetColor(const Color &color);
    void SetIntensity(float intensity);
    void SetShadowBias(float shadowBias);
    void SetShadowType(ShadowType shadowType);

    Color GetColor() const;
    float GetIntensity() const;
    float GetShadowBias() const;
    ShadowType GetShadowType() const;

    void RenderShadowMaps();

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Light();
    virtual ~Light();

    void SetLightMaterial(Material* lightMat);

    virtual void SetUniformsBeforeApplyingLight(Material* mat) const;

private:
    float m_intensity = 1.0f;
    Color m_color = Color::White;

    float m_shadowBias = 0.01f;
    ShadowType m_shadowType = ShadowType::SOFT;

    RH<Material> p_lightMaterial;

    void ApplyLight(Camera *camera, const AARect &renderRect) const;
    virtual AARect GetRenderRect(Camera *camera) const;
    virtual void RenderShadowMaps_() = 0;

    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // LIGHT_H
