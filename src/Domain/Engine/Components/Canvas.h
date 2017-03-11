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
    virtual ICloneable *Clone() const override;

    virtual void Read(const XMLNode *xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Canvas();
    virtual ~Canvas();

    friend class GameObject;
};

#endif // CANVAS_H
