#ifndef UILAYOUTMANAGER_H
#define UILAYOUTMANAGER_H

#include "Bang/Bang.h"

#include "Bang/Map.h"
#include "Bang/Axis.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Component;
FORWARD class RectTransform;
FORWARD class ILayoutElement;
FORWARD class ILayoutController;
FORWARD class IRectTransformListener;

class UILayoutManager
{
public:
    UILayoutManager();

    static void OnInvalidated(ILayoutElement    *element);
    static void OnInvalidated(ILayoutController *controller);

    static Vector2i GetMinSize(GameObject *go);
    static Vector2i GetPreferredSize(GameObject *go);
    static Vector2  GetFlexibleSize(GameObject *go);
    static Vector2  GetSize(GameObject *go, LayoutSizeType sizeType);

    static void RebuildLayout(GameObject *gameObject);

    static List<GameObject*> GetLayoutableChildrenList(GameObject *go);

private:
    static void CalculateLayout(GameObject *gameObject, Axis axis);
    static void ApplyLayout(GameObject *gameObject, Axis axis);

    static void OnLayoutRebuilt(GameObject *go);
    static void OnInvalidatedLayout(Component *comp,
                                   bool isLayoutController);

    static UILayoutManager *GetInstance();
};

NAMESPACE_BANG_END

#endif // UILAYOUTMANAGER_H
