#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/Component.h"

FORWARD class XMLNode;
FORWARD class ICloneable;

class Canvas : public Component
{
    OBJECT(Canvas)

public:
    virtual void CloneInto(ICloneable *clone) const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    Canvas();
    virtual ~Canvas();
};

#endif // CANVAS_H
