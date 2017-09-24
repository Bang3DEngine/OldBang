#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIButton;
FORWARD class GameObject;
FORWARD class UIGameObject;

class GameObjectFactory
{
public:
    static GameObject* CreateGameObject(const String &gameObjectClassName);
    static bool ExistsGameObjectClass(const String &gameObjectClassName);

    static GameObject* CreateGameObject(bool addTransform = true);
    static UIGameObject* CreateUIGameObject(bool addRectTransform = true);

    static UIGameObject* CreateGUIInputText();
    static UIButton*     CreateGUIButton();
    static UIGameObject* CreateGUILabel(const String &content = "Bang");
    static UIGameObject* CreateGUIScrollArea();
    static UIGameObject* CreateGUISpacer();
    static UIGameObject* CreateGUIHSpacer();
    static UIGameObject* CreateGUIVSpacer();

    GameObjectFactory() = delete;
};

NAMESPACE_BANG_END

#endif // GAMEOBJECTFACTORY_H
