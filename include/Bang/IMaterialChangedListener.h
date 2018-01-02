#ifndef IMATERIALCHANGEDLISTENER_H
#define IMATERIALCHANGEDLISTENER_H

#include "Bang/IEventListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Material;

class IMaterialChangedListener : public virtual IEventListener
{
public:
    virtual void OnMaterialChanged(const Material *changedMaterial) = 0;
};

NAMESPACE_BANG_END

#endif // IMATERIALCHANGEDLISTENER_H
