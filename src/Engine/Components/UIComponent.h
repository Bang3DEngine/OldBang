#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "Bang/Component.h"
#include "Bang/IFocusable.h"

FORWARD class UIGameObject;

class UIComponent : public Component,
                    public IFocusable
{
public:
    virtual ~UIComponent();

    UIGameObject *GetGameObject() const;


protected:
    UIComponent();
};

#endif // UICOMPONENT_H
