﻿#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/Serializable.h"

NAMESPACE_BANG_BEGIN

#define IRESOURCE(CLASSNAME) friend class Resources;

#define RESOURCE(CLASSNAME)\
    IRESOURCE(CLASSNAME) \
    ICLONEABLE(CLASSNAME) \
    SERIALIZABLE(CLASSNAME)

class IResource
{
    IRESOURCE(IResource)

protected:
    IResource();
    virtual ~IResource();

    friend class Resources;
};

class Resource : public virtual Serializable,
                 public IResource
{
    IRESOURCE(Resource)

public:
    Path GetResourceFilepath() const;
    virtual void Import(const Path &resourceFilepath) = 0;

protected:
    Resource() = default;
    virtual ~Resource() = default;
};

NAMESPACE_BANG_END

#endif // RESOURCE_H
