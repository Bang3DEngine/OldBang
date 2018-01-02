#ifndef UIRENDERERCACHER_H
#define UIRENDERERCACHER_H

#include <unordered_map>

#include "Bang/GLEnums.h"
#include "Bang/Component.h"
#include "Bang/RenderPass.h"
#include "Bang/UIRenderer.h"
#include "Bang/IChildrenListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Framebuffer;
FORWARD class UIImageRenderer;

class UIRendererCacher : public Component,
                         public IChildrenListener,
                         public IUIRendererChangeListener
{
    COMPONENT( UIRendererCacher )

public:
	UIRendererCacher();
	virtual ~UIRendererCacher();

    void OnStart() override;
    void OnBeforeRender() override;
    void OnAfterChildrenRender(RenderPass renderPass) override;

    void SetCachingEnabled(bool enabled);

    bool IsCachingEnabled() const;
    GameObject *GetContainer() const;

    // IChildrenListener
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;

    // IUIRendererChangeListener
    void OnUIRendererChanged(const UIRenderer *changedUIRenderer) override;

private:
    bool m_cachingEnabled = true;
    bool m_needNewImageToSnapshot = true;

    std::unordered_map<UIRenderer*, bool> m_uiRenderersVisibility;

    Framebuffer *p_cacheFramebuffer = nullptr;
    UIImageRenderer *p_cachedImageRenderer = nullptr;
    GameObject *p_uiRenderersContainer = nullptr;

    void SnapshotGBufferIntoCachedImage();

    void RestoreContainerUIRenderersVisibility();
    void SetContainerUIRenderersInvisible();
    static UIRendererCacher* CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIRENDERERCACHER_H

