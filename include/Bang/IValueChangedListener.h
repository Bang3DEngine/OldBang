#ifndef IVALUECHANGEDLISTENER_H
#define IVALUECHANGEDLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class IEventEmitter;

class IValueChangedListener: public IEventListener
{
public:
    virtual void OnValueChanged(const IEventEmitter *emitter) = 0;
};

NAMESPACE_BANG_END

#endif // IVALUECHANGEDLISTENER_H
