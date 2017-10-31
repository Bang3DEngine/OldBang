#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;

class UICanvas : public Component
{
    COMPONENT(UICanvas)

public:
    UICanvas();
    virtual ~UICanvas();

    virtual void OnUpdate() override;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;
};

NAMESPACE_BANG_END

#endif // CANVAS_H
