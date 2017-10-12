#ifndef ICHILDRENLISTENER_H
#define ICHILDRENLISTENER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IChildrenListener
{
public:
    virtual void OnChildrenAdded() = 0;
    virtual void OnChildrenRemoved() = 0;
};

NAMESPACE_BANG_END

#endif // ICHILDRENLISTENER_H
