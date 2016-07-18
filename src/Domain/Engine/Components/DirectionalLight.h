#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "GBuffer.h"
#include "Component.h"

class DirectionalLight : public Component
{
friend class Scene;

private:
    Vector3 color;
    Material *material = nullptr;

    void ApplyLight(GBuffer *gbuffer);

public:
    DirectionalLight();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Directional Light"; }
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
    #endif

    void Write(std::ostream &f) const;
    void Read(std::istream &f);
};

#endif // DIRECTIONALLIGHT_H
