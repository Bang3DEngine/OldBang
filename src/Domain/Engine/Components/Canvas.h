#ifndef CANVAS_H
#define CANVAS_H

#include "Component.h"

class Canvas : public Component
{
protected:
    Canvas();
    virtual ~Canvas();

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

    friend class GameObject;
};

#endif // CANVAS_H
