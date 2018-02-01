#ifndef UIRENDERERCACHER_H
#define UIRENDERERCACHER_H

#include <unordered_map>

#include "Bang/Component.h"
#include "Bang/RenderPass.h"
#include "Bang/UIRenderer.h"
#include "Bang/IChildrenListener.h"
#include "Bang/IRendererChangedListener.h"
#include "Bang/IGameObjectVisibilityChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Framebuffer;
FORWARD class ShaderProgram;
FORWARD class UIImageRenderer;

class UIRendererCacher : public Component,
                         public IChildrenListener,
                         public ITransformListener,
                         public IRendererChangedListener,
                         public IGameObjectVisibilityChangedListener
{
    COMPONENT( UIRendererCacher )

public:
	UIRendererCacher();
	virtual ~UIRendererCacher();

    void OnStart() override;
    void OnRender(RenderPass renderPass) override;
    void OnAfterChildrenRender(RenderPass renderPass) override;

    void SetCachingEnabled(bool enabled);

    bool IsCachingEnabled() const;
    GameObject *GetContainer() const;

    void OnChanged();

    // IChildrenListener
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;

    // ITransformListener
    void OnTransformChanged() override;

    // IRendererChangedListener
    void OnRendererChanged(Renderer *changedRenderer) override;

    // IObjectListener
    void OnStarted() override;
    void OnEnabled() override;
    void OnDisabled() override;

private:
    bool m_cachingEnabled = true;
    bool m_needNewImageToSnapshot = true;

    Framebuffer *p_cacheFramebuffer = nullptr;
    GameObject *p_uiRenderersContainer = nullptr;
    UIImageRenderer *p_cachedImageRenderer = nullptr;

    void SetContainerVisible(bool visible);
    static UIRendererCacher* CreateInto(GameObject *go);

    // IGameObjectVisibilityChangedListener
    void OnVisibilityChanged(GameObject *go) override;

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIRENDERERCACHER_H

