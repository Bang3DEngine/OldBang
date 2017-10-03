#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/LayoutSizeType.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class GameObject;
FORWARD class UIGameObject;
FORWARD class UIButtonDriver;

class GameObjectFactory
{
public:
    static GameObject* CreateGameObject(const String &gameObjectClassName);
    static bool ExistsGameObjectClass(const String &gameObjectClassName);

    static GameObject* CreateGameObject(bool addTransform = true);
    static UIGameObject* CreateUIGameObject(bool addRectTransform = true);
    static Scene* CreateScene();
    static Scene* CreateUIScene();
    static Scene* CreateDefaultScene();

    static UIGameObject*   CreateGUIInputText();
    static UIButtonDriver* CreateGUIButton();
    static UIGameObject*   CreateGUILabel(const String &content = "Bang");
    static UIGameObject*   CreateGUIScrollArea();

    static UIGameObject* CreateGUIHSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceY = 1);
    static UIGameObject* CreateGUIVSeparator(
                            LayoutSizeType sizeType = LayoutSizeType::Preferred,
                            int spaceX = 1);

    static UIGameObject* CreateGUISpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            const Vector2i &space = Vector2i(1));
    static UIGameObject* CreateGUIHSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceX = 1);
    static UIGameObject* CreateGUIVSpacer(
                            LayoutSizeType sizeType = LayoutSizeType::Flexible,
                            int spaceY = 1);

    GameObjectFactory() = delete;

private:
    static UIGameObject* CreateGUISeparator(LayoutSizeType sizeType,
                                            const Vector2i &space);

};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H
