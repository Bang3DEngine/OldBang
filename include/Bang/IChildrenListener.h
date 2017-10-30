#ifndef ICHILDRENLISTENER_H
#define ICHILDRENLISTENER_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class IChildrenListener
{
public:
    virtual void OnChildAdded(GameObject *addedChild)     {};
    virtual void OnChildRemoved(GameObject *removedChild) {};
    virtual void OnParentChanged(GameObject *oldParent,
                                 GameObject *newParent)   {};
};

NAMESPACE_BANG_END

#endif // ICHILDRENLISTENER_H