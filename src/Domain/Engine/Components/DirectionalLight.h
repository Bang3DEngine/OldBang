#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{

public:
    DirectionalLight();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Directional Light"; }

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
    #endif

    virtual void Write(std::ostream &f) const;
    virtual void Read(std::istream &f);
};

#endif // DIRECTIONALLIGHT_H
