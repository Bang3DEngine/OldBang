#include "Bang/UIRendererCacher.h"

#include "Bang/Camera.h"
#include "Bang/GBuffer.h"
#include "Bang/GEngine.h"
#include "Bang/GameObject.h"
#include "Bang/UIRenderer.h"
#include "Bang/Framebuffer.h"
#include "Bang/RectTransform.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

UIRendererCacher::UIRendererCacher()
{
    p_cacheFramebuffer = new Framebuffer(1, 1);
    p_cacheFramebuffer->Bind();
    p_cacheFramebuffer->CreateAttachment(GL_Attachment::Color0,
                                         GL_ColorFormat::RGBA_UByte8);
    p_cacheFramebuffer->UnBind();
}

UIRendererCacher::~UIRendererCacher()
{
    delete p_cacheFramebuffer;
}

void UIRendererCacher::OnStart()
{
    // Prepare image renderer
    Texture2D *tex = p_cacheFramebuffer->GetAttachmentTexture(GL_Attachment::Color0);
    if (p_cachedImageRenderer)
    {
        tex->SetWrapMode(GL_WrapMode::Repeat);
        p_cachedImageRenderer->SetImageTexture(tex);
        p_cachedImageRenderer->SetTint(Color::White);
        p_cachedImageRenderer->SetUvMultiply(Vector2(1, -1));
        p_cachedImageRenderer->GetImageTexture()->SetAlphaCutoff(1.0f);
        p_cachedImageRenderer->GetImageTexture()->SetFilterMode(GL_FilterMode::Nearest);
    }
}

void UIRendererCacher::OnRender(RenderPass rp)
{
    Component::OnRender(rp);

    if (rp == RenderPass::Canvas)
    {
        if (IsCachingEnabled() && m_needNewImageToSnapshotInNextFrame)
        {
            m_needNewImageToSnapshot = true;
            m_needNewImageToSnapshotInNextFrame = false;
        }
        else if (!IsCachingEnabled())
        {
            RestoreContainerUIRenderersVisibility();
        }

        if (m_needNewImageToSnapshot) { RestoreContainerUIRenderersVisibility(); }
        else { SetContainerUIRenderersInvisible(); }

        p_cachedImageRenderer->SetVisible(!m_needNewImageToSnapshot);
    }
}

void UIRendererCacher::OnAfterChildrenRender(RenderPass renderPass)
{
    Component::OnAfterChildrenRender(renderPass);
    if (renderPass == RenderPass::Canvas &&
        IsCachingEnabled() &&
        m_needNewImageToSnapshot)
    {
        SnapshotGBufferIntoCachedImage();
        m_needNewImageToSnapshot = false;
        m_needNewImageToSnapshotInNextFrame = false;
    }
}

void UIRendererCacher::SetCachingEnabled(bool enabled)
{
    if (enabled != IsCachingEnabled())
    {
        m_cachingEnabled = enabled;
        p_cachedImageRenderer->SetEnabled( IsCachingEnabled() );
    }
}

bool UIRendererCacher::IsCachingEnabled() const
{
    return m_cachingEnabled;
}

GameObject *UIRendererCacher::GetContainer() const
{
    return p_uiRenderersContainer;
}

void UIRendererCacher::OnDestroyed(Object *object)
{
    UIRenderer *uiRend = DCAST<UIRenderer*>(object);
    ASSERT(uiRend);

    auto it = m_uiRenderersVisibility.find(uiRend);
    if (it != m_uiRenderersVisibility.end())
    {
        m_uiRenderersVisibility.erase(it);
    }
}

void UIRendererCacher::OnChildAdded(GameObject *addedChild, GameObject *parent)
{
    List<GameObject*> children = GetContainer()->GetChildrenRecursively();
    for (GameObject *child : children)
    {
        List<UIRenderer*> uiRenderers = child->GetComponents<UIRenderer>();
        for (UIRenderer *uiRend : uiRenderers)
        {
            uiRend->EventEmitter<IRendererChangedListener>::RegisterListener(this);
        }
        child->EventEmitter<IChildrenListener>::RegisterListener(this);
    }
}

void UIRendererCacher::OnChildRemoved(GameObject *removedChild, GameObject *parent)
{
    List<GameObject*> children = removedChild->GetChildrenRecursively();
    children.PushBack(removedChild);
    for (GameObject *child : children)
    {
        List<UIRenderer*> uiRenderers = child->GetComponents<UIRenderer>();
        for (UIRenderer *uiRend : uiRenderers)
        {
            uiRend->EventEmitter<IRendererChangedListener>::UnRegisterListener(this);
        }
        child->EventEmitter<IChildrenListener>::UnRegisterListener(this);
    }
}

void UIRendererCacher::OnRendererChanged(const Renderer*)
{
    m_needNewImageToSnapshotInNextFrame = true;
}

void UIRendererCacher::SnapshotGBufferIntoCachedImage()
{
    ASSERT(!p_cachedImageRenderer->IsVisible());

    // Save previous state
    GLId prevBoundDrawFramebuffer = GL::GetBoundId(GL_BindTarget::DrawFramebuffer);
    GLId prevBoundReadFramebuffer = GL::GetBoundId(GL_BindTarget::ReadFramebuffer);
    Array<GL_Attachment> prevDrawAttachments = GL::GetDrawBuffers();
    GL_Attachment prevReadAttachment = GL::GetReadBuffer();

    // Get src and destination rects
    Rect rtRectPx = GetGameObject()->GetRectTransform()->GetViewportRect();
    p_cacheFramebuffer->Resize(rtRectPx.GetWidth(), rtRectPx.GetHeight());

    Recti dstRectPx(Vector2i::Zero, Vector2i(p_cacheFramebuffer->GetSize()));
    GBuffer *gbuffer = GEngine::GetActive()->GetActiveCamera()->GetGBuffer();

    // Bind read and draw framebuffers
    GL::Bind(GL_BindTarget::ReadFramebuffer, gbuffer->GetGLId());
    GL::ReadBuffer(GBuffer::AttColor);

    GL::Bind(GL_BindTarget::DrawFramebuffer, p_cacheFramebuffer->GetGLId());
    GL::DrawBuffers( {GL_Attachment::Color0} );

    GL::Disable(GL_Test::Blend);

    // Copy from GBuffer to cache framebuffer
    GL::BlitFramebuffer(Recti(rtRectPx),
                        dstRectPx,
                        GL_FilterMode::Nearest,
                        GL_BufferBit::Color);

    GL::Enable(GL_Test::Blend);

    // Restore
    GL::Bind(GL_BindTarget::DrawFramebuffer, prevBoundDrawFramebuffer);
    GL::Bind(GL_BindTarget::ReadFramebuffer, prevBoundReadFramebuffer);
    GL::DrawBuffers(prevDrawAttachments);
    GL::ReadBuffer(prevReadAttachment);

    p_cacheFramebuffer->GetAttachmentTexture(GL_Attachment::Color0)->
            ToImage<Byte>().Export( Path(GetGameObject()->GetName() + ".png") );
}

void UIRendererCacher::RestoreContainerUIRenderersVisibility()
{
    IRendererChangedListener::SetReceiveEvents(false);

    for (const auto &uiRend_visibility : m_uiRenderersVisibility)
    {
        UIRenderer *uiRend = uiRend_visibility.first;
        bool wasVisible = uiRend_visibility.second;
        uiRend->SetVisible(wasVisible);
    }
    m_uiRenderersVisibility.clear();

    IRendererChangedListener::SetReceiveEvents(true);
}

void UIRendererCacher::SetContainerUIRenderersInvisible()
{
    IRendererChangedListener::SetReceiveEvents(false);

    bool isANewInvisibilitySet = m_uiRenderersVisibility.empty();
    if (isANewInvisibilitySet)
    {
        List<UIRenderer*> containerUIRenderers =
                    GetContainer()->GetComponentsInChildren<UIRenderer>(true);
        for (UIRenderer *uiRend : containerUIRenderers)
        {
            uiRend->EventEmitter<IDestroyListener>::RegisterListener(this);
            m_uiRenderersVisibility[uiRend] = uiRend->IsVisible();
            uiRend->SetVisible(false);
        }
    }

    IRendererChangedListener::SetReceiveEvents(true);
}

UIRendererCacher* UIRendererCacher::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UIRendererCacher *rendererCacher = go->AddComponent<UIRendererCacher>();
    UIImageRenderer *cacherImageRend = go->AddComponent<UIImageRenderer>();
    cacherImageRend->SetVisible(false);

    GameObject *container = GameObjectFactory::CreateUIGameObject();

    rendererCacher->p_cachedImageRenderer = cacherImageRend;
    rendererCacher->p_uiRenderersContainer = container;

    container->SetParent(go);
    container->EventEmitter<IChildrenListener>::RegisterListener(rendererCacher);
    rendererCacher->OnChildAdded(container, go);

    return rendererCacher;
}
