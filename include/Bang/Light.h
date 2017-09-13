#ifndef LIGHT_H
#define LIGHT_H

#include "Bang/Color.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;
FORWARD class Material;
FORWARD class Renderer;
FORWARD class GBuffer;

class Light : public Component
{
    COMPONENT(Light)

public:
    virtual ~Light();

    void SetColor(const Color &color);
    void SetIntensity(float intensity);

    Color GetColor() const;
    float GetIntensity() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    Light();
    void SetLightMaterial(Material *lightMat);

    virtual void SetUniformsBeforeApplyingLight(Material *mat) const;

private:
    float m_intensity = 1.0f;
    Color m_color = Color::White;
    Material *m_lightMaterialScreen = nullptr;

    void ApplyLight(GBuffer *gbuffer, const Rect &renderRect) const;
    virtual Rect GetRenderRect(Camera *cam) const;

    friend class GEngine;
};

NAMESPACE_BANG_END

#endif // LIGHT_H
