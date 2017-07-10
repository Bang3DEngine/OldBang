#ifndef COMPONENTINSPECTABLE_H
#define COMPONENTINSPECTABLE_H

#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IconManager.h"
#include "Bang/Inspectable.h"
#include "Bang/ComponentWidget.h"

template <>
class Inspectable<Component> : public IInspectable
{
    ICLONEABLE(Inspectable<Component>)

public:
    Inspectable()
    {
    }

    Inspectable(Component *component)
    {
        SetRelatedSerializableObject(component);
    }

    virtual void CloneInto(ICloneable *clone) const override
    {
        IInspectable::CloneInto(clone);
    }

    virtual bool IsEnabled() const override
    {
        return GetComponent()->IsEnabled();
    }

    virtual bool NeedsEnabledCheckBox() const override
    {
        return true;
    }

    virtual void OnEnabledChanged(bool enabled) override
    {
        GetComponent()->SetEnabled(enabled);
    }

    virtual String GetTitleInInspector() const override
    {
        return GetComponent()->GetClassName();
    }

    virtual InspectorWidget *GetNewInspectorWidget() override
    {
        ComponentWidget *cw = new ComponentWidget( GetComponent() );
        return cw;
    }

    virtual List<IInspectable*> GetNewInspectablesToShow() override
    {
        return {};
    }

    virtual const QPixmap& GetIcon() const
    {
        const Component *c = GetComponent();
        return IconManager::GetIcon( c ? c->GetClassName() : "" );
    }

private:
    Component *GetComponent() const
    {
        return Object::SCast<Component>( GetSerializableObject() );
    }

};

#endif // COMPONENTINSPECTABLE_H
