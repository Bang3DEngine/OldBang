#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/Flags.h"
#include "Bang/String.h"
#include "Bang/ICloneable.h"
#include "Bang/Destroyable.h"

#define OBJECT_NO_FRIEND(CLASS) \
        ICLONEABLE(CLASS)\
        public: \
        virtual String GetClassName() const override { return #CLASS; } \
        static String GetClassNameStatic() { return #CLASS; }

#define OBJECT(CLASS) \
        OBJECT_NO_FRIEND(CLASS) \
        friend class GameObject;

class Object : public ICloneable,
               public Destroyable
{
public:
    Object();
    virtual ~Object();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String GetClassName() const = 0;
    virtual String GetInstanceId() const;

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

};

#endif // OBJECT_H
