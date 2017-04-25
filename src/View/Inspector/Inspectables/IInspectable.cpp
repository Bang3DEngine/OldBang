#include "IInspectable.h"

IInspectable::IInspectable()
{
}

IInspectable::~IInspectable()
{
}

void IInspectable::SetSerializableObject(SerializableObject *serializableObject)
{
    p_relatedSerializableObject = serializableObject;
}

SerializableObject *IInspectable::GetRelatedSerializableObject() const
{
    return p_relatedSerializableObject;
}
