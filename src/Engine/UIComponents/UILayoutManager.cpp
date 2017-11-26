#include "Bang/UILayoutManager.h"

#include "Bang/Set.h"
#include "Bang/Rect.h"
#include "Bang/Scene.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/Application.h"
#include "Bang/SceneManager.h"
#include "Bang/RectTransform.h"
#include "Bang/ILayoutElement.h"
#include "Bang/UILayoutIgnorer.h"
#include "Bang/ILayoutController.h"
#include "Bang/ITransformListener.h"
#include "Bang/ILayoutSelfController.h"

USING_NAMESPACE_BANG

UILayoutManager::UILayoutManager()
{
    ObjectManager::RegisterCreateListener(this);
}

void UILayoutManager::OnInvalidated(ILayoutElement *element)
{
    Component *comp = Cast<Component*>(element);
    UILayoutManager::OnLayoutInvalidated(comp, false);

    UILayoutManager *lmgr = UILayoutManager::GetLayoutManagerFor(comp->GetGameObject());
    if (lmgr)
    {
        lmgr->m_invalidatedLayoutElements.PushBack(element);
        lmgr->OnInvalidated(comp->GetGameObject());
    }
}

void UILayoutManager::OnInvalidated(ILayoutController *controller)
{
    Component *comp = Cast<Component*>(controller);
    UILayoutManager::OnLayoutInvalidated(comp, true);

    UILayoutManager *lmgr = UILayoutManager::GetLayoutManagerFor(comp->GetGameObject());
    if (lmgr)
    {
        lmgr->m_invalidatedLayoutControllers.PushBack(controller);
        lmgr->OnInvalidated(comp->GetGameObject());
    }
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
    Map<int, List<ILayoutElement*> > priorLayoutElms;
    List<ILayoutElement*> les = go->GetComponents<ILayoutElement>();
    for (ILayoutElement *le : les)
    {
        int prior = le->GetLayoutPriority();
        if (!priorLayoutElms.ContainsKey(prior))
        {
            priorLayoutElms.Add(prior, List<ILayoutElement*>());
        }
        priorLayoutElms.Get(prior).PushBack(le);
    }
    if (priorLayoutElms.IsEmpty()) { return Vector2::Zero; }

    // Get the max size between the elements ordered by priority.
    // Sizes less than zero will be ignored.
    Vector2 size = Vector2(-1);
    bool sizeXFound = false, sizeYFound = false;
    for (auto it = priorLayoutElms.RBegin(); it != priorLayoutElms.REnd(); ++it)
    {
        const List<ILayoutElement*> &les = (*it).second;
        for (ILayoutElement *le : les)
        {
            Vector2 leSize =  le->GetSize(sizeType);
            if (!sizeXFound) { size.x = Math::Max(size.x, leSize.x); }
            if (!sizeYFound) { size.y = Math::Max(size.y, leSize.y); }
        }

        if (size.x >= 0) { sizeXFound = true; }
        if (size.y >= 0) { sizeYFound = true; }
        if (sizeXFound && sizeYFound) { break; }
    }
    return Vector2::Max(size, Vector2::Zero);
}

List<GameObject *> UILayoutManager::GetLayoutableChildrenList(GameObject *go)
{
    List<GameObject*> childrenList;
    for (GameObject *child : go->GetChildren())
    {
        UILayoutIgnorer *ltIgnorer = child->GetComponent<UILayoutIgnorer>();
        bool ignoreLayout = ltIgnorer ? ltIgnorer->IsIgnoreLayout() : false;
        if (child->IsEnabled() &&
            child->HasComponent<ILayoutElement>() && !ignoreLayout)
        {
            childrenList.PushBack(child);
        }
    }
    return childrenList;
}

void UILayoutManager::OnCreated(Object *object)
{
    GameObject *go = Cast<GameObject*>(object);
    if (go) { m_invalidatedGameObjects.Add(go); }
}

void UILayoutManager::RebuildLayout(GameObject *rootGo)
{
    ENSURE(rootGo);
    CalculateLayout(rootGo, Axis::Horizontal);
    ApplyLayout(rootGo, Axis::Horizontal);
    CalculateLayout(rootGo, Axis::Vertical);
    ApplyLayout(rootGo, Axis::Vertical);

    m_invalidatedGameObjects.Clear();
    m_invalidatedLayoutElements.Clear();
    m_invalidatedLayoutControllers.Clear();
}

void UILayoutManager::CalculateLayout(GameObject *gameObject, Axis axis)
{
    if (!m_invalidatedGameObjects.Contains(gameObject)) { return; }

    const List<GameObject*> &children = gameObject->GetChildren();
    for (GameObject *child : children)
    {
        CalculateLayout(child, axis);
    }

    List<ILayoutElement*> goLEs = gameObject->GetComponents<ILayoutElement>();
    for (ILayoutElement *goLE : goLEs)
    {
        goLE->_CalculateLayout(axis);
    }
}

void UILayoutManager::ApplyLayout(GameObject *gameObject, Axis axis)
{
    if (!m_invalidatedGameObjects.Contains(gameObject)) { return; }

    std::queue<String> indentQueue; indentQueue.push("");

    std::queue<GameObject*> goQueue; goQueue.push(gameObject);
    while (!goQueue.empty())
    {
        GameObject *go = goQueue.front(); goQueue.pop();

        List<ILayoutController*> nonSelfControllers;
        List<ILayoutController*> layoutControllers =
                                        go->GetComponents<ILayoutController>();

        // SelfLayoutControllers
        for (ILayoutController *layoutController : layoutControllers)
        {
            ILayoutSelfController *selfController =
                           Cast<ILayoutSelfController*>(layoutController);
            if (selfController)
            {
                selfController->_ApplyLayout(axis);
            }
            else { nonSelfControllers.PushBack(layoutController); }
        }

        // Normal LayoutControllers
        for (ILayoutController *layoutController : nonSelfControllers)
        {
            layoutController->_ApplyLayout(axis);
        }

        const List<GameObject*> &children = go->GetChildren();
        for (GameObject *child : children) { goQueue.push(child); }
    }
}

void UILayoutManager::OnInvalidated(GameObject *go)
{
    if (go && !m_invalidatedGameObjects.Contains(go))
    {
        m_invalidatedGameObjects.Add(go);
        OnInvalidated(go->GetParent());
    }
}

void UILayoutManager::OnLayoutInvalidated(Component *comp,
                                          bool isLayoutController)
{
    ENSURE(comp);

    GameObject *go = comp->GetGameObject();

    if (isLayoutController)
    {
        auto pLayoutContrs = go->GetComponentsInParent<ILayoutController>(false);
        for (ILayoutController *pCont : pLayoutContrs) { pCont->Invalidate(); }

        auto thisLayoutElms = go->GetComponents<ILayoutElement>();
        for (ILayoutElement *lElm : thisLayoutElms) { lElm->Invalidate(); }
    }

    auto cLayoutContrs = go->GetComponentsInChildrenOnly<ILayoutController>(false);
    for (ILayoutController *cCont : cLayoutContrs) { cCont->Invalidate(); }

    const bool isLayoutElement = !isLayoutController;
    if (isLayoutElement)
    {
        auto pLayoutContrs = go->GetComponentsInParent<ILayoutController>(false);
        for (ILayoutController *pCont : pLayoutContrs) { pCont->Invalidate(); }

        auto pLayoutElements = go->GetComponentsInParent<ILayoutElement>(false);
        for (ILayoutElement *pElm : pLayoutElements) { pElm->Invalidate(); }

        auto cLayoutElements = go->GetComponentsInChildrenOnly<ILayoutElement>(false);
        for (ILayoutElement *cElm : cLayoutElements) { cElm->Invalidate(); }

        auto thisLayoutContrs = go->GetComponents<ILayoutController>();
        for (ILayoutController *lContr : thisLayoutContrs) { lContr->Invalidate(); }
    }
}

UILayoutManager *UILayoutManager::GetLayoutManagerFor(GameObject *go)
{
    UICanvas *canvas = go->GetComponentInParent<UICanvas>(true);
    return canvas ? canvas->GetLayoutManager() : nullptr;
}

