#ifndef CANVAS_H
#define CANVAS_H

#include "String.h"
#include "Component.h"

class XMLNode;
class ICloneable;
class Canvas : public Component
{
public:

    virtual const String ToString() const override;
    virtual String GetName() const override { return "Canvas"; }

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    Canvas();
    virtual ~Canvas();

    friend class GameObject;
};

#endif // CANVAS_H
