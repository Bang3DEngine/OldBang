#ifndef RESOURCE_H
#define RESOURCE_H

#include "Bang/SerializableObject.h"

#define RESOURCE(CLASSNAME) SERIALIZABLE_OBJECT(CLASSNAME)

class Resource : public SerializableObject
{
public:
    virtual ~Resource() {}

protected:
    Resource() {}
};

#endif // RESOURCE_H
