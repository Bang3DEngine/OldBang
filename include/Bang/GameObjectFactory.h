#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "Bang/String.h"

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
    static UIGameObject* CreateGUIButton();
    static UIGameObject* CreateGUILabel(const String &content = "Bang");
    static UIGameObject* CreateGUIScrollArea();

    GameObjectFactory() = delete;
};

#endif // GAMEOBJECTFACTORY_H
