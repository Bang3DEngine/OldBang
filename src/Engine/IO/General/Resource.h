#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/SerializableObject.h"

#define RESOURCE(CLASSNAME) SERIALIZABLE_OBJECT(CLASSNAME)

class Resource : public SerializableObject
{
public:
    virtual ~Resource();

    Path GetResourceFilepath() const;
    virtual void Import(const Path &resourceFilepath) = 0;

protected:
    Resource();
};

#endif // RESOURCE_H
