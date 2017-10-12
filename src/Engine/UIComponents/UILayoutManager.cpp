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
    std::queue<GameObject*> goQueue; goQueue.push(rootGo);
    while (!goQueue.empty())
    {
        GameObject *go = goQueue.front();

        List<ILayoutController*> nonSelfControllers;
        auto layoutControllers = go->GetComponents<ILayoutController>();

        // First SelfControllers
        for (ILayoutController *layoutController : layoutControllers)
        {
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
        for (GameObject *child : go->GetChildren()) { goQueue.push(child); }
    }
}

UILayoutManager *UILayoutManager::GetInstance()
{
    Scene *scene = SceneManager::GetRootScene();
    return scene ? scene->GetUILayoutManager() : nullptr;
}
