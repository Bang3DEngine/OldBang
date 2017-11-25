#ifndef UILAYOUTMANAGER_H
#define UILAYOUTMANAGER_H

#include "Bang/Bang.h"

#include "Bang/Map.h"
#include "Bang/Set.h"
#include "Bang/Axis.h"
#include "Bang/List.h"
#include "Bang/LayoutSizeType.h"
#include "Bang/ICreateListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Component;
FORWARD class RectTransform;
FORWARD class ILayoutElement;
FORWARD class ILayoutController;
FORWARD class IRectTransformListener;

class UILayoutManager : public ICreateListener
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

    // ICreateListener
    virtual void OnCreated(Object *object) override;

private:
    Set<GameObject*> m_invalidatedGameObjects;
    List<ILayoutElement*> m_invalidatedLayoutElements;
    List<ILayoutController*> m_invalidatedLayoutControllers;

    void CalculateLayout(GameObject *gameObject, Axis axis);
    void ApplyLayout(GameObject *gameObject, Axis axis);

    void OnInvalidated(GameObject *go);

    static void OnLayoutRebuilt(GameObject *go);
    static void OnLayoutInvalidated(Component *comp, bool isLayoutController);

    static UILayoutManager* GetLayoutManagerFor(GameObject *go);
};

NAMESPACE_BANG_END

#endif // UILAYOUTMANAGER_H
