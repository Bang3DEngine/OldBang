#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/String.h"
#include "Bang/Component.h"

class XMLNode;
class ICloneable;
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
