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
#include "Bang/ILayoutController.h"
#include "Bang/IRectTransformListener.h"
#include "Bang/ILayoutSelfController.h"

USING_NAMESPACE_BANG

UILayoutManager::UILayoutManager()
{
}

void UILayoutManager::OnInvalidated(ILayoutElement *element)
{
    UILayoutManager::OnInvalidatedLayout( DCAST<Component*>(element), false );
}

void UILayoutManager::OnInvalidated(ILayoutController *controller)
{
    UILayoutManager::OnInvalidatedLayout( DCAST<Component*>(controller), true );
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

void UILayoutManager::RebuildLayout(GameObject *rootGo)
{
    ENSURE(rootGo);
    std::queue<String> indentQueue; indentQueue.push("");
    std::queue<GameObject*> goQueue; goQueue.push(rootGo);
    while (!goQueue.empty())
    {
        String currentIndent = indentQueue.front();
        GameObject *go = goQueue.front();

        List<ILayoutController*> nonSelfControllers;
        auto layoutControllers = go->GetComponents<ILayoutController>();
        bool valid = true;

        // First SelfControllers
        for (ILayoutController *layoutController : layoutControllers)
        {
            if (layoutController->IsInvalid()) { valid = false; }
            if (DCAST<ILayoutSelfController*>(layoutController))
            {
                layoutController->ApplyLayout();
            }
            else { nonSelfControllers.PushBack(layoutController); }
        }

        // Then, "normal" controllers
        for (ILayoutController *layoutController : nonSelfControllers)
        {
            layoutController->ApplyLayout();
        }

        goQueue.pop();
        indentQueue.pop();
        for (GameObject *child : go->GetChildren()) { goQueue.push(child); indentQueue.push(currentIndent + "  "); }
    }
}

List<GameObject *> UILayoutManager::GetLayoutableChildrenList(GameObject *go)
{
    List<GameObject*> childrenList;
    for (GameObject *child : go->GetChildren())
    {
        bool addIt = false;
        List<ILayoutElement*> lElms = child->GetComponents<ILayoutElement>();
        for (ILayoutElement *le : lElms)
        {
            if (!le->GetIgnoreLayout()) { addIt = true; break; }
        }
        if (addIt) { childrenList.PushBack(child); }
    }
    return childrenList;
}

void UILayoutManager::OnInvalidatedLayout(Component *comp,
                                          bool isLayoutController)
{
    ENSURE(comp);

    GameObject *go = comp->GetGameObject();

    if (true) // isLayoutController)
    {
        auto pLayoutContrs = go->GetComponentsInParent<ILayoutController>(false);
        for (ILayoutController *pCont : pLayoutContrs) { pCont->Invalidate(); }

        auto thisLayoutElms = go->GetComponents<ILayoutElement>();
        for (ILayoutElement *lElm : thisLayoutElms) { lElm->Invalidate(); }
    }

    auto cLayoutContrs = go->GetComponentsInChildrenOnly<ILayoutController>(false);
    for (ILayoutController *cCont : cLayoutContrs) { cCont->Invalidate(); }

    const bool isLayoutElement = !isLayoutController;
    if (true) // isLayoutElement)
    {
        auto pLayoutElements = go->GetComponentsInParent<ILayoutElement>(false);
        for (ILayoutElement *pElm : pLayoutElements) { pElm->Invalidate(); }

        auto cLayoutElements = go->GetComponentsInChildrenOnly<ILayoutElement>(false);
        for (ILayoutElement *cElm : cLayoutElements) { cElm->Invalidate(); }

        auto thisLayoutContrs = go->GetComponents<ILayoutController>();
        for (ILayoutController *lContr : thisLayoutContrs) { lContr->Invalidate(); }
    }
}

UILayoutManager *UILayoutManager::GetInstance()
{
    Scene *scene = SceneManager::GetRootScene();
    return scene ? scene->GetUILayoutManager() : nullptr;
}
