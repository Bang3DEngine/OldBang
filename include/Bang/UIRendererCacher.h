#ifndef UIRENDERERCACHER_H
#define UIRENDERERCACHER_H

#include <unordered_map>

#include "Bang/GLEnums.h"
#include "Bang/Component.h"
#include "Bang/RenderPass.h"
#include "Bang/UIRenderer.h"
#include "Bang/IChildrenListener.h"
#include "Bang/IRendererChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Framebuffer;
FORWARD class UIImageRenderer;

class UIRendererCacher : public Component,
                         public IChildrenListener,
                         public IRendererChangedListener
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

    // IChildrenListener
    void OnChildAdded(GameObject *addedChild, GameObject *parent) override;
    void OnChildRemoved(GameObject *removedChild, GameObject *parent) override;

    // IRendererChangedListener
    void OnRendererChanged(Renderer *changedRenderer) override;

private:
    bool m_cachingEnabled = true;
    bool m_needNewImageToSnapshot = true;
    bool m_needNewImageToSnapshotInNextFrame = true;

    Framebuffer *p_cacheFramebuffer = nullptr;
    GameObject *p_uiRenderersContainer = nullptr;
    UIImageRenderer *p_cachedImageRenderer = nullptr;

    void SnapshotGBufferIntoCachedImage();

    void SetContainerEnabled(bool enabled);
    static UIRendererCacher* CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UIRENDERERCACHER_H

