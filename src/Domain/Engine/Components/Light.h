#ifndef LIGHT_H
#define LIGHT_H

#include "Rect.h"
#include "Color.h"
#include "Component.h"

class Camera;
class GBuffer;
class Material;
class Renderer;
class Light : public Component
{
public:

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    void SetColor(const Color &color);
    void SetIntensity(float intensity);

    Color GetColor() const;
    float GetIntensity() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    float m_intensity = 1.0f;
    Color m_color = Color::White;
    Material *m_lightMaterialScreen = nullptr;

    virtual void SetUniformsBeforeApplyingLight(Material *mat) const;
    void ApplyLight(GBuffer *gbuffer,
                    const Rect &renderRect = Rect::ScreenRect) const;

    /**
     * @brief Returns the rect where the Light range can apply.
     * This is for performance, the deferred light will only be applied
     * to the intersection of this rect and the object/s we are applying
     * the light to. For example, a point light must return a bounding rect
     * of its spherical range.
     * @return
     */
    virtual Rect GetRenderRect(Camera *cam) const;

    Light();
    virtual ~Light();

    friend class GameObject;
    friend class GraphicPipeline;
};

#endif // LIGHT_H
