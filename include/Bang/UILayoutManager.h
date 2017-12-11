#ifndef UILAYOUTMANAGER_H
#define UILAYOUTMANAGER_H

#include "Bang/Bang.h"

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/Axis.h"
#include "Bang/List.h"
#include "Bang/Object.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Component;
FORWARD class RectTransform;
FORWARD class ILayoutElement;
FORWARD class ILayoutController;
FORWARD class ITransformListener;

class UILayoutManager
{
public:
    UILayoutManager();

    void RebuildLayout(GameObject *gameObject);

    static void OnInvalidated(ILayoutElement    *element);
    static void OnInvalidated(ILayoutController *controller);

    static Vector2i GetMinSize(GameObject *go);
    static Vector2i GetPreferredSize(GameObject *go);
    static Vector2  GetFlexibleSize(GameObject *go);
    static Vector2  GetSize(GameObject *go, LayoutSizeType sizeType);

    static List<GameObject*> GetLayoutableChildrenList(GameObject *go);

private:
    void CalculateLayout(GameObject *gameObject, Axis axis);
    void ApplyLayout(GameObject *gameObject, Axis axis);

    static void OnLayoutRebuilt(GameObject *go);
    static void OnLayoutInvalidated(Component *comp, bool isLayoutController);

    static UILayoutManager* GetActive(GameObject *go);
    static UILayoutManager* GetActive(Component *comp);
};

NAMESPACE_BANG_END

#endif // UILAYOUTMANAGER_H
