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
    obj->GetHideFlags()->SetOn( GetHideFlags().ToInteger() );
}

HideFlags* Object::GetHideFlags()
{
    return &m_hideFlags;
}

const HideFlags& Object::GetHideFlags() const
{
    return m_hideFlags;
}

String Object::GetInstanceId() const
{
    return String::ToString( static_cast<const void*>(this) );
}

