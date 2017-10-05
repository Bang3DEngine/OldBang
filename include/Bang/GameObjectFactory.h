#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class GameObject;
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

    static UIListDriver* CreateGUIList(bool vertical);
    static GameObject*   CreateGUIInputText();
    static UIButtonDriver* CreateGUIButton();
    static GameObject*   CreateGUILabel(const String &content = "Bang");
    static UIScrollArea* CreateGUIScrollAreaInto(GameObject *go);
    static UIScrollArea* CreateGUIScrollArea();

    static GameObject* CreateGUIHSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceY = 1);
    static GameObject* CreateGUIVSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceX = 1);

    static GameObject* CreateGUISpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            const Vector2i &space = Vector2i(1));
    static GameObject* CreateGUIHSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceX = 1);
    static GameObject* CreateGUIVSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceY = 1);

    GameObjectFactory() = delete;

private:
    static GameObject* CreateGUISeparator(LayoutSizeType sizeType,
                                          const Vector2i &space);

};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H
