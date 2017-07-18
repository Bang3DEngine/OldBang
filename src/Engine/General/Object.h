#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/Flags.h"
#include "Bang/String.h"
#include "Bang/ICloneable.h"

#define OBJECT_NO_FRIEND(CLASS) \
        ICLONEABLE(CLASS)\
        public: \
        virtual String GetClassName() const override { return #CLASS; } \
        static String GetClassNameStatic() { return #CLASS; }

#define OBJECT(CLASS) \
        OBJECT_NO_FRIEND(CLASS) \
        friend class GameObject;

class Object : public ICloneable
{
public:
    Object();
    virtual ~Object();

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String GetClassName() const = 0;
    virtual String GetInstanceId() const;
};

#endif // OBJECT_H
