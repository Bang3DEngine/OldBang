#ifndef UIMASK_H
#define UIMASK_H

#include "Bang/GL.h"
#include "Bang/Array.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIMask : public Component
{
    COMPONENT(UIMask)

public:
    UIMask();
    virtual ~UIMask();

    virtual void OnRender(RenderPass renderPass) override;
    virtual void OnBeforeChildrenRender(RenderPass renderPass) override;
    virtual void OnAfterChildrenRender(RenderPass renderPass) override;

    void SetMasking(bool maskEnabled);
    void SetDrawMask(bool drawMask);

    bool IsMasking() const;
    bool IsDrawMask() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    bool m_masking = true;
    bool m_drawMask = false;
    bool m_restoringStencil = false;

    Array<BoolByte> m_maskBefore;
    GL::Function  m_stencilFuncBefore;
    GL::StencilOperation m_stencilOpBefore;

    void PrepareStencilToDrawMask();
    void PrepareStencilToDrawChildren();
    void RestoreStencilBuffer(RenderPass renderPass);
};

NAMESPACE_BANG_END

#endif // UIMASK_H
