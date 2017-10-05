#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class UILabel;
FORWARD class GameObject;
FORWARD class UIInputText;
FORWARD class UIListDriver;
FORWARD class UIScrollArea;
FORWARD class UIButtonDriver;

class GameObjectFactory
{
public:
    static GameObject* CreateGameObject(bool addTransform = true);
    static GameObject* CreateUIGameObject(bool addRectTransform = true);

    static Scene* CreateScene();
    static Scene* CreateUIScene();
    static Scene* CreateDefaultScene();

    static UIListDriver*   CreateUIListInto(GameObject *go);
    static UIListDriver*   CreateUIList();
    static UIInputText*    CreateUIInputTextInto(GameObject *go);
    static UIInputText*    CreateUIInputText();
    static UIButtonDriver* CreateUIButtonInto(GameObject *go);
    static UIButtonDriver* CreateUIButton();
    static UILabel*        CreateUILabel();
    static UILabel*        CreateUILabelInto(GameObject *go);
    static UIScrollArea*   CreateUIScrollAreaInto(GameObject *go);
    static UIScrollArea*   CreateUIScrollArea();

    static GameObject* CreateUIHSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceY = 1);
    static GameObject* CreateUIVSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceX = 1);

    static GameObject* CreateUISpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            const Vector2i &space = Vector2i(1));
    static GameObject* CreateUIHSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceX = 1);
    static GameObject* CreateUIVSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceY = 1);

    GameObjectFactory() = delete;

private:
    static GameObject* CreateGUISeparator(LayoutSizeType sizeType,
                                          const Vector2i &space);

};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H
