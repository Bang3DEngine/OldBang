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

Vector2i UILayoutManager::GetSize(GameObject *go, LayoutSizeType sizeType)
{
    int maxPriorFound = -1; // If less than zero, ignore it
    ILayoutElement *maxPriorLayoutElement = nullptr;
    List<ILayoutElement*> les = go->GetComponents<ILayoutElement>();
    for (ILayoutElement *le : les)
    {
        if (le->GetPriority() > maxPriorFound)
        {
            maxPriorFound = le->GetPriority();
            maxPriorLayoutElement = le;
        }
    }

    if (maxPriorLayoutElement)
    {
        return maxPriorLayoutElement->GetTotalSize(sizeType);
    }
    return Vector2i::Zero;
}

void UILayoutManager::InvalidateDown(RectTransform *rt)
{
    ENSURE(rt);
    if (!rt->IsInvalid())
    {
        rt->SetInvalid(true);
        GameObject *rtGo = rt->GetGameObject();
        for (GameObject *child : rtGo->GetChildren())
        {
            RectTransform *crt = child->GetComponent<RectTransform>();
            UILayoutManager::InvalidateDown(crt);
        }
    }
}

void UILayoutManager::InvalidateLayoutUpwards(GameObject *go)
{
    ENSURE(go);
    List<ILayoutElement*> layoutElms = go->GetComponents<ILayoutElement>();
    for (ILayoutElement *layoutElm : layoutElms) { layoutElm->SetInvalid(true); }

    if (go->parent) { UILayoutManager::InvalidateLayoutUpwards(go->parent); }
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

void UILayoutManager::RebuildLayout(GameObject *_go)
{
    ENSURE(_go);
    std::queue<GameObject*> goQueue; goQueue.push(_go);
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
