#ifndef CANVAS_H
#define CANVAS_H

#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class GameObject;
FORWARD class UILayoutManager;

class UICanvas : public Component
{
    COMPONENT(UICanvas)

public:
    UICanvas();
    virtual ~UICanvas();

    void Invalidate();

    virtual void OnStart() override;
    virtual void OnUpdate() override;
    virtual void OnPostUpdate() override;

    virtual void CloneInto(ICloneable *clone) const override;

    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    UILayoutManager* GetLayoutManager() const;

private:
    UILayoutManager *m_uiLayoutManager = nullptr;

};

NAMESPACE_BANG_END

#endif // CANVAS_H
