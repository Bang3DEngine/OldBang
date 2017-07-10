#include "IInspectable.h"

#include "Bang/Object.h"

IInspectable::IInspectable()
{
}

IInspectable::~IInspectable()
{
}

void IInspectable::CloneInto(ICloneable *clone) const
{
    IInspectable *iinsp = Object::SCast<IInspectable>(clone);
    iinsp->SetRelatedSerializableObject( GetSerializableObject() );
}

void IInspectable::SetRelatedSerializableObject(SerializableObject *serializableObject)
{
    p_serializableObject = serializableObject;
}

SerializableObject *IInspectable::GetSerializableObject() const
{
    return p_serializableObject;
}
