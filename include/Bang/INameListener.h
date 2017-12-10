#ifndef INAMELISTENER_H
#define INAMELISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

class INameListener : public virtual IEventListener
{
public:
    virtual void OnNameChanged(GameObject *go, const String &oldName,
                               const String &newName) = 0;
};

NAMESPACE_BANG_END

#endif // ICHILDRENLISTENER_H
