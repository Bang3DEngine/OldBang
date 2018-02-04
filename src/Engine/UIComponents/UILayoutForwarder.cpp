#include "Bang/UILayoutForwarder.h"

#include "Bang/GameObject.h"
#include "Bang/UILayoutManager.h"

USING_NAMESPACE_BANG

UILayoutForwarder::UILayoutForwarder()
{
}

UILayoutForwarder::~UILayoutForwarder()
{
}

void UILayoutForwarder::CalculateLayout(Axis axis)
{
    List<GameObject*> childLayoutElmsGo =
            UILayoutManager::GetLayoutableChildrenList(GetGameObject());

    Vector2i minSize  =  Vector2i::Zero;
    Vector2i prefSize = -Vector2i::One;
    Vector2  flexSize = -Vector2::One;
    for (GameObject *leGo : childLayoutElmsGo)
    {
        Vector2i childMinSize (
               UILayoutManager::GetSize(leGo, LayoutSizeType::Min));
        Vector2i childPrefSize(
               UILayoutManager::GetSize(leGo, LayoutSizeType::Preferred));
        Vector2  childFlexSize(
               UILayoutManager::GetSize(leGo, LayoutSizeType::Flexible));

        minSize  = Vector2i::Max(minSize,  childMinSize);
        prefSize = Vector2i::Max(prefSize, childPrefSize);
        flexSize =  Vector2::Max(flexSize, childFlexSize);
    }

    SetCalculatedLayout(axis,
                        minSize.GetAxis(axis),
                        prefSize.GetAxis(axis),
                        flexSize.GetAxis(axis));
}

