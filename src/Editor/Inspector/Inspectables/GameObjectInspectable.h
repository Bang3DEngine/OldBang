#ifndef GAMEOBJECTINSPECTABLE_H
#define GAMEOBJECTINSPECTABLE_H

#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/Inspectable.h"
#include "Bang/ComponentInspectable.h"

template <>
class Inspectable<GameObject> : public IInspectable
{
    ICLONEABLE(Inspectable<GameObject>)

public:
    Inspectable(GameObject *gameObject)
    {
        SetRelatedSerializableObject(gameObject);
    }

    virtual bool IsEnabled() const override
    {
        return GetGameObject()->IsEnabled();
    }

    virtual bool NeedsEnabledCheckBox() const override
    {
        return true;
    }

    virtual void OnEnabledChanged(bool enabled) override
    {
        GetGameObject()->SetEnabled(enabled);
    }

    virtual String GetTitleInInspector() const override
    {
        return GetGameObject()->GetName();
    }

    virtual InspectorWidget *GetNewInspectorWidget() override
    {
        return nullptr;
    }

    virtual List<IInspectable*> GetNewInspectablesToShow() override
    {
        if (!GetGameObject()) { return List<IInspectable*>(); }

        List<IInspectable*> componentsInspectables;
        for (Component *comp : GetGameObject()->GetComponents())
        {
            Inspectable<Component> *compInsp = new Inspectable<Component>(comp);
            componentsInspectables.Add(compInsp);
        }
        return componentsInspectables;
    }

protected:
    Inspectable() {}

    virtual GameObject *GetGameObject() const
    {
        return Object::SCast<GameObject>( GetSerializableObject() );
    }

};

#endif // GAMEOBJECTINSPECTABLE_H
