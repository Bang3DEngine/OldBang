#ifndef LIGHT_H
#define LIGHT_H

#include "GBuffer.h"
#include "Component.h"
#include "Gizmos.h"

#ifdef BANG_EDITOR
#include "InspectorWidget.h"
#endif

class Light : public Component
{
friend class Scene;
friend class GameObject;

protected:
    float m_intensity = 1.0f;
    Color m_color = Color::white;
    Material *m_lightMaterial = nullptr;

    virtual void SetUniformsBeforeApplyingLight() const;
    virtual void ApplyLight(GBuffer *gbuffer) const;

    Light();

public:

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Directional Light"; }

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    void SetColor(const Color &color);
    void SetIntensity(float intensity);

    Color GetColor() const;
    float GetIntensity() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // LIGHT_H
