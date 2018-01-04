#ifndef UIRECTMASK_H
#define UIRECTMASK_H

#include "Bang/GL.h"
#include "Bang/Array.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIRectMask : public Component
{
    COMPONENT(UIRectMask)

public:
    UIRectMask();
    virtual ~UIRectMask();

    void OnBeforeChildrenRender(RenderPass renderPass) override;
    void OnAfterChildrenRender(RenderPass renderPass) override;

    void SetMasking(bool maskEnabled);

    bool IsMasking() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    bool m_wasScissorEnabled = false;
    Recti m_prevScissor = Recti::Zero;

    bool m_masking = true;
};

NAMESPACE_BANG_END

#endif // UIRECTMASK_H
