#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Math.h"
#include "Light.h"
#include "String.h"

class XMLNode;
class ICloneable;
class DirectionalLight : public Light
{
public:

    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    virtual void OnDrawGizmos() override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

protected:
    DirectionalLight();
    virtual ~DirectionalLight();

    friend class GameObject;

};

#endif // DIRECTIONALLIGHT_H
