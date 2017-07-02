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
    iinsp->SetRelatedSerializableObject( GetRelatedSerializableObject() );
}

void IInspectable::SetRelatedSerializableObject(SerializableObject *serializableObject)
{
    p_relatedSerializableObject = serializableObject;
}

SerializableObject *IInspectable::GetRelatedSerializableObject() const
{
    return p_relatedSerializableObject;
}
