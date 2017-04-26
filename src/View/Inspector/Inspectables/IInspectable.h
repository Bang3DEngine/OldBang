#ifndef IINSPECTABLE_H
#define IINSPECTABLE_H

#include "Bang/List.h"
#include "Bang/ICloneable.h"

class InspectorWidget;
class SerializableObject;
class IInspectable : public ICloneable
{
public:
    virtual ~IInspectable();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual bool IsEnabled() const = 0;
    virtual bool NeedsEnabledCheckBox() const = 0;
    virtual void OnEnabledChanged(bool enabled) = 0;
    virtual String GetTitleInInspector() const = 0;
    virtual InspectorWidget *GetNewInspectorWidget() = 0;
    virtual List<IInspectable*> GetNewInspectablesToShow() = 0;

    void SetRelatedSerializableObject(SerializableObject* serializableObject);

    SerializableObject *GetRelatedSerializableObject() const;

protected:
    IInspectable();

private:
    SerializableObject *p_relatedSerializableObject = nullptr;
};

#endif // IINSPECTABLE_H
