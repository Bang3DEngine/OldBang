#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/Serializable.h"

#define RESOURCE(CLASSNAME) SERIALIZABLE(CLASSNAME)

class Resource : public Serializable
{
public:
    virtual ~Resource();

    Path GetResourceFilepath() const;
    virtual void Import(const Path &resourceFilepath) = 0;

protected:
    Resource();
};

#endif // RESOURCE_H
