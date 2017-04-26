#ifndef INSPECTABLE_H
#define INSPECTABLE_H

#include "Bang/List.h"
#include "Bang/IInspectable.h"
#include "Bang/InspectorWidget.h"
#include "Bang/SerializableObject.h"

template <class SerializableObjectClass>
class Inspectable : public IInspectable
{
    ICLONEABLE(Inspectable<SerializableObjectClass>)

public:
    Inspectable()
    {
    }

    virtual void CloneInto(ICloneable *clone) const override
    {
        IInspectable::CloneInto(clone);
    }

    Inspectable(SerializableObjectClass *relatedSerializableObject)
    {
        SetRelatedSerializableObject(relatedSerializableObject);
    }

    virtual ~Inspectable()
    {
    }

    virtual bool IsEnabled() const override              { return true;  }
    virtual bool NeedsEnabledCheckBox() const override   { return false; }
    virtual void OnEnabledChanged(bool enabled) override { }

    virtual String GetTitleInInspector() const override
    {
        return "Inspectable";
    }

    virtual InspectorWidget *GetNewInspectorWidget() override
    {
        InspectorWidget *iw = new InspectorWidget();
        iw->Init(GetTitleInInspector(), GetRelatedSerializableObject());
        return iw;
    }

    virtual List<IInspectable*> GetNewInspectablesToShow() override
    {
        return {this};
    }
};

#endif // INSPECTABLE_H
