#ifndef ICLONEABLE_H
#define ICLONEABLE_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

#define ICLONEABLE(CLASS) \
    public: \
    virtual CLASS* Clone() const override { \
        CLASS *c = new CLASS(); \
        CloneInto(c); \
        return c; \
    }

class ICloneable
{
public:
    virtual ICloneable* Clone() const
    {
        ASSERT_MSG(false, "Method not implemented");
    }

protected:
    ICloneable() {}
    virtual ~ICloneable() {}
    virtual void CloneInto(ICloneable *clone) const = 0;
};

NAMESPACE_BANG_END

#endif // ICLONEABLE_H
