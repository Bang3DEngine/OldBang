#ifndef CANVAS_H
#define CANVAS_H

#include "String.h"
#include "Component.h"

class XMLNode;
class ICloneable;
class Canvas : public Component
{
public:

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    ICloneable *CloneVirtual() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Canvas();
    virtual ~Canvas();

    friend class GameObject;
    friend class ICloneable;
};

#endif // CANVAS_H
