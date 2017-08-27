#ifndef UIMASK_H
#define UIMASK_H

#include "Bang/GL.h"
#include "Bang/UIGameObject.h"

class GUIMask : public UIGameObject
{
    UIGAMEOBJECT(GUIMask)

public:
    GUIMask();
    virtual ~GUIMask();

    virtual void Render(RenderPass renderPass, bool renderChildren) override;
    virtual void OnRender(RenderPass renderPass) override;

    void SetMasking(bool maskEnabled);
    void SetDrawMask(bool drawMask);

    bool GetMasking() const;
    bool GetDrawMask() const;

private:
    bool m_masking = true;
    bool m_drawMask = false;
    bool m_restoringStencil = false;

    GLenum m_stencilOpBefore;
    bool m_maskRBefore = true, m_maskGBefore = true,
         m_maskBBefore = true, m_maskABefore = true;
    bool m_stencilTestBefore = false;
    bool m_stencilWriteBefore = false;

    void BeforeThisRender();
    void BeforeChildrenRender();
    void AfterChildrenRender(RenderPass renderPass);
};

#endif // UIMASK_H
