#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Bang/Light.h"

NAMESPACE_BANG_BEGIN

FORWARD class Camera;

class PointLight : public Light
{
    COMPONENT(PointLight)

public:
    void SetRange(float range);
    float GetRange() const;

    // Component
    void OnRender(RenderPass rp) override;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

protected:
    float m_range = 1.0f;

    PointLight();
    virtual ~PointLight();

    // Light
    virtual void SetUniformsBeforeApplyingLight(Material* mat) const override;

    // Renderer
    Rect GetRenderRect(Camera *cam) const override;
};

NAMESPACE_BANG_END

#endif // POINTLIGHT_H
