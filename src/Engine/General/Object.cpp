#include "Bang/Object.h"

#include <typeinfo>

Object::Object()
{

}

Object::~Object()
{

}

void Object::CloneInto(ICloneable *clone) const
{
    Object *obj = SCAST<Object*>(clone);
}

String Object::GetInstanceId() const
{
    return String::ToString( static_cast<const void*>(this) );
}

