#ifndef UIMASK_H
#define UIMASK_H

#include "Bang/GL.h"
#include "Bang/Component.h"

class UIMask : public Component
{
    COMPONENT(UIMask)

public:
    UIMask();
    virtual ~UIMask();

    virtual void OnRender(RenderPass renderPass) override;
    virtual void OnBeforeChildrenRender(RenderPass renderPass) override;
    virtual void OnChildrenRendered(RenderPass renderPass) override;

    void SetMasking(bool maskEnabled);
    void SetDrawMask(bool drawMask);

    bool GetMasking() const;
    bool GetDrawMask() const;

    // SerializableObject
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    bool m_masking = true;
    bool m_drawMask = false;
    bool m_restoringStencil = false;

    GLenum m_stencilOpBefore;
    bool m_maskRBefore = true, m_maskGBefore = true,
         m_maskBBefore = true, m_maskABefore = true;
    bool m_stencilTestBefore = false;
    bool m_stencilWriteBefore = false;

    void PrepareStencilToDrawMask();
    void PrepareStencilToDrawChildren();
    void RestoreStencilBuffer(RenderPass renderPass);
};

#endif // UIMASK_H
