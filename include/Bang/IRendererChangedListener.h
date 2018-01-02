#ifndef IRENDERERCHANGEDLISTENER_H
#define IRENDERERCHANGEDLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Renderer;

class IRendererChangedListener : public virtual IEventListener
{
public:
    virtual void OnRendererChanged(const Renderer *changedRenderer) = 0;
};

NAMESPACE_BANG_END

#endif // IRENDERERCHANGEDLISTENER_H
