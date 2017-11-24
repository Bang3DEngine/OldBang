#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Bang/Color.h"
#include "Bang/Renderer.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

class UIRenderer : public Renderer
{
public:
    virtual Rect GetBoundingRect(Camera *camera) const override;

    // Renderer
    virtual void OnRender() override;
    virtual void OnRender(RenderPass renderPass) override;

    void SetCullByRectTransform(bool cullByRectTransform);
    bool GetCullByRectTransform() const;

protected:
    UIRenderer();
    virtual ~UIRenderer();

private:
    bool m_cullByRectTransform = true;
};

NAMESPACE_BANG_END

#endif // UIRENDERER_H
