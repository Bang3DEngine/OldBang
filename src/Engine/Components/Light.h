#ifndef LIGHT_H
#define LIGHT_H

#include "Bang/Color.h"
#include "Bang/Component.h"

FORWARD   class Camera;
FORWARD   class Material;
FORWARD   class Renderer;
FORWARD   class G_GBuffer;

class Light : public Component
{
    OBJECT(Light)

public:
    virtual void CloneInto(ICloneable *clone) const override;

    void SetColor(const Color &color);
    void SetIntensity(float intensity);

    Color GetColor() const;
    float GetIntensity() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    float m_intensity = 1.0f;
    Color m_color = Color::White;
    Material *m_lightMaterialScreen = nullptr;

    virtual void SetUniformsBeforeApplyingLight(Material *mat) const;
    void ApplyLight(G_GBuffer *gbuffer, const Rect &renderRect) const;

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

    friend class GraphicPipeline;
};

#endif // LIGHT_H
