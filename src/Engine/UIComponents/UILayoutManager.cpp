#include "Bang/UILayoutManager.h"

#include "Bang/Scene.h"
#include "Bang/UICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/UILayoutElement.h"

USING_NAMESPACE_BANG

void UILayoutManager::RebuildLayout(Scene *scene, bool force)
{
    ENSURE(scene);

    List<UILayoutElement*> layoutElements;
    layoutElements = scene->GetComponentsInChildren<UILayoutElement>();

    if (force)
    {
        for (UILayoutElement *layoutElement : layoutElements)
        {
            layoutElement->SetDirty(true);
        }
        scene->RecalculateLayout();
    }
    else
    {
        for (UILayoutElement *layoutElement : layoutElements)
        {
            if (layoutElement->IsDirty())
            {
                layoutElement->gameObject->RecalculateLayout();
            }
        }
    }
}
