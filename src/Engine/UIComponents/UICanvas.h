#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/Component.h"

FORWARD class UIGameObject;

class UICanvas : public Component
{
    COMPONENT(UICanvas)

public:
    UICanvas();
    virtual ~UICanvas();

    virtual void OnUpdate() override;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    UIGameObject *GetFocusedGameObject() const;
    void GiveFocusTo(UIGameObject *focusTaker) const;

private:
    mutable UIGameObject *p_focus = nullptr;
};

#endif // CANVAS_H
