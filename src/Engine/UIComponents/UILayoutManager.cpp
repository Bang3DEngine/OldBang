#include "Bang/UILayoutManager.h"

#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/ILayoutElement.h"
#include "Bang/ILayoutController.h"
#include "Bang/IRectTransformListener.h"

USING_NAMESPACE_BANG

UILayoutManager::UILayoutManager()
{
}

Vector2i UILayoutManager::GetMinSize(GameObject *go)
{
    return Vector2i( UILayoutManager::GetSize(go, LayoutSizeType::Min) );
}

Vector2i UILayoutManager::GetPreferredSize(GameObject *go)
{
    return Vector2i( UILayoutManager::GetSize(go, LayoutSizeType::Preferred) );
}

Vector2 UILayoutManager::GetFlexibleSize(GameObject *go)
{
    return UILayoutManager::GetSize(go, LayoutSizeType::Flexible);
}

Vector2 UILayoutManager::GetSize(GameObject *go, LayoutSizeType sizeType)
{
    // Retrieve layout elements and their respective priority
    Map<int, List<ILayoutElement*> > priorLayoutElements;
    List<ILayoutElement*> les = go->GetComponents<ILayoutElement>();
    for (ILayoutElement *le : les)
    {
        int prior = le->GetLayoutPriority();
        if (!priorLayoutElements.ContainsKey(prior))
        {
            priorLayoutElements.Add(prior, List<ILayoutElement*>());
        }
        priorLayoutElements.Get(prior).PushBack(le);
    }
    if (priorLayoutElements.IsEmpty()) { return Vector2::Zero; }

    // Get the max size between the elements ordered by priority.
    // Sizes less than zero will be ignored.
    Vector2 size = Vector2(-1);
    bool sizeXFound = false, sizeYFound = false;
    for (auto it = priorLayoutElements.RBegin();
         it != priorLayoutElements.REnd(); ++it)
    {
        const List<ILayoutElement*> &les = (*it).second;
        for (ILayoutElement *le : les)
        {
            Vector2 leSize =  le->GetTotalSize(sizeType);
            if (!sizeXFound) { size.x = Math::Max(size.x, leSize.x); }
            if (!sizeYFound) { size.y = Math::Max(size.y, leSize.y); }
        }

        if (size.x >= 0) { sizeXFound = true; }
        if (size.y >= 0) { sizeYFound = true; }
        if (sizeXFound && sizeYFound) { break; }
    }
    return Vector2::Max(size, Vector2::Zero);
}

void UILayoutManager::Invalidate(RectTransform *rt)
{
    ENSURE(rt);
    if (!rt->IsInvalid())
    {
        rt->SetInvalid(true);
        GameObject *rtGo = rt->GetGameObject();
        for (GameObject *child : rtGo->GetChildren())
        {
            RectTransform *crt = child->GetComponent<RectTransform>();
            UILayoutManager::Invalidate(crt);
        }
    }
}

void UILayoutManager::InvalidateAll(GameObject *go)
{
    ENSURE(go);
    List<RectTransform*> rectTransforms =
                            go->GetComponentsInChildren<RectTransform>();
    for (RectTransform *rt : rectTransforms) { rt->InvalidateDown(); }

    List<ILayoutElement*> layoutElms =
                            go->GetComponentsInChildren<ILayoutElement>();
    for (ILayoutElement *le : layoutElms) { le->SetInvalid(true); }

    UILayoutManager::GetInstance()->m_previousFrameRectTransforms.Clear();
}

void UILayoutManager::RebuildLayout(GameObject *rootGo)
{
    ENSURE(rootGo);
    std::queue<GameObject*> goQueue; goQueue.push(rootGo);
    while (!goQueue.empty())
    {
        GameObject *go = goQueue.front();
        List<ILayoutController*> layoutControllers =
                         go->GetComponents<ILayoutController>();
        for (ILayoutController *layoutController : layoutControllers)
        {
            layoutController->ApplyLayout();
        }
        goQueue.pop();
        for (GameObject *child : go->GetChildren()) { goQueue.push(child); }
    }
}

void UILayoutManager::ForceRebuildLayout(GameObject *go)
{
    UILayoutManager::InvalidateAll(go);
    UILayoutManager::RebuildLayout(go);
}

void UILayoutManager::OnBeforeRender(GameObject *go)
{
    RectTransform *goRT = go->GetComponent<RectTransform>();
    if (goRT)
    {
        Recti newRect = goRT->GetScreenSpaceRectPx();
        List<IRectTransformListener*> rectTransformListeners =
                go->GetComponents<IRectTransformListener>();
        for (IRectTransformListener *rtListener : rectTransformListeners)
        {
            if (!m_previousFrameRectTransforms.ContainsKey(rtListener))
            {
                m_previousFrameRectTransforms.Add(rtListener, Recti::Zero);
            }

            const Recti &prevRect = m_previousFrameRectTransforms.Get(rtListener);
            if (prevRect != newRect)
            {
                m_previousFrameRectTransforms.Add(rtListener, newRect);
                rtListener->OnRectTransformChanged();
            }
        }
    }

    for (GameObject *child : go->GetChildren()) { OnBeforeRender(child); }
}

UILayoutManager *UILayoutManager::GetInstance()
{
    Scene *scene = SceneManager::GetRootScene();
    return scene ? scene->GetUILayoutManager() : nullptr;
}
