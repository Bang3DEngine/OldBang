#ifndef OBJECT_H
#define OBJECT_H

#include "String.h"
#include "HideFlags.h"

class Object
{
public:
    Object();
    virtual ~Object();

    const HideFlags &GetHideFlags() const;
    bool HasHideFlag(const HideFlags &hideFlag) const;

    void AddHideFlag(const HideFlags &hideFlag);
    void RemoveHideFlag(const HideFlags &hideFlag);

    String GetInstanceId() const;

    template <class TO, class FROM>
    static bool IsOfType(const FROM obj)
    {
        return dynamic_cast<const TO*>(obj) != nullptr;
    }

    template <class TO, class FROM>
    static TO* Cast(FROM obj)
    {
        return dynamic_cast<TO*>(obj);
    }

    template <class TO, class FROM>
    static const TO* ConstCast(FROM obj)
    {
        return dynamic_cast<const TO*>(obj);
    }

    template <class TO, class FROM>
    static TO* SCast(FROM *obj)
    {
        return static_cast<TO*>(obj);
    }

    template <class TO, class FROM>
    static const TO* SConstCast(const FROM *obj)
    {
        return static_cast<const TO*>(obj);
    }

    template <class T>
    bool IsOfType() const { return Object::IsOfType<T>(this); }
    template <class T>
    T *SCast() { return Object::SCast<T>(this); }
    template <class T>
    const T *SConstCast() const { return Object::SCast<const T>(this); }
    template <class T>
    T *Cast() { return Object::Cast<T>(this); }
    template <class T>
    const T *ConstCast() const { return Object::Cast<const T>(this); }

private:
    HideFlags m_hideFlags = HideFlags::None;
};

#endif // OBJECT_H
