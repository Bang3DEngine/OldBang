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
    Object *obj = Object::SCast<Object>(clone);
}

String Object::GetInstanceId() const
{
    return String::ToString( static_cast<const void*>(this) );
}

