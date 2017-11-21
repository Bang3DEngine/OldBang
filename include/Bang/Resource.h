#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/Serializable.h"

NAMESPACE_BANG_BEGIN

#define RESOURCE(CLASSNAME)\
    ICLONEABLE(CLASSNAME) \
    SERIALIZABLE(CLASSNAME)

class Resource : public virtual Serializable
{
public:
    Path GetResourceFilepath() const;
    virtual void Import(const Path &resourceFilepath) = 0;

protected:
    Resource() = default;
    virtual ~Resource() = default;

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // RESOURCE_H
