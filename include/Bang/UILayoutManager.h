#ifndef UILAYOUTMANAGER_H
#define UILAYOUTMANAGER_H

#include "Bang/Bang.h"

#include "Bang/Map.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class RectTransform;
FORWARD class IRectTransformListener;

class UILayoutManager
{
public:
    UILayoutManager();

    static Vector2i GetSize(GameObject *go,
                            LayoutSizeType sizeType);

    static void Invalidate(RectTransform *rt);
    static void InvalidateAll(GameObject *gameObject);
    static void RebuildLayout(GameObject *gameObject);
    static void ForceRebuildLayout(GameObject *gameObject);

    void OnBeforeRender(GameObject *go);

private:
    Map<IRectTransformListener*, Recti> m_previousFrameRectTransforms;

    static void OnLayoutRebuilt(GameObject *go);
    static UILayoutManager *GetInstance();
};

NAMESPACE_BANG_END

#endif // UILAYOUTMANAGER_H
