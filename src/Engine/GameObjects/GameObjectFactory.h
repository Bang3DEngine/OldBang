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

    static UIGameObject* CreateGUIInputText();
    static UIGameObject* CreateGUIButton();

    template<class T>
    static T* CreateGameObject();

    GameObjectFactory() = delete;
};

template<class T>
T* GameObjectFactory::CreateGameObject()
{
    return new T();
}

#endif // GAMEOBJECTFACTORY_H
