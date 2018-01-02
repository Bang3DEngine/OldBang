#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Bang/Color.h"
#include "Bang/Renderer.h"
#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IEventListener.h"
#include "Bang/IEnabledListener.h"
#include "Bang/IChildrenListener.h"
#include "Bang/ITransformListener.h"
#include "Bang/IMaterialChangedListener.h"

NAMESPACE_BANG_BEGIN

class UIRenderer : public Renderer,
                   public IChildrenListener,
                   public ITransformListener
{
public:
    virtual Rect GetBoundingRect(Camera *camera) const override;

    // Renderer
    virtual void OnRender() override;
    virtual void OnRender(RenderPass renderPass) override;

    void SetCullByRectTransform(bool cullByRectTransform);
    bool GetCullByRectTransform() const;

    // IEnabledListener
    virtual void OnEnabled()  override;
    virtual void OnDisabled() override;

    // ITransformListener
    virtual void OnTransformChanged() override;

    // IChildrenListener
    virtual void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    virtual void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;
    virtual void OnParentChanged(GameObject *oldParent, GameObject *newParent) override;

protected:
    UIRenderer();
    virtual ~UIRenderer();

private:
    bool m_cullByRectTransform = true;

};

NAMESPACE_BANG_END

#endif // UIRENDERER_H
