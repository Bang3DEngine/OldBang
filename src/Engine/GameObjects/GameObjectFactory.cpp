#include "Bang/GameObjectFactory.h"

#include "Bang/GUIMask.h"
#include "Bang/Material.h"
#include "Bang/GUIButton.h"
#include "Bang/GUICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/GUIInputText.h"
#include "Bang/UIGameObject.h"
#include "Bang/GUIScrollArea.h"
#include "Bang/GUITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"

#define CREATE_GAMEOBJECT(className, GameObjectClass) \
    if (className == GameObjectClass::GetClassNameStatic()) \
    { return new GameObjectClass(); }

GameObject*
GameObjectFactory::CreateGameObject(const String &gameObjectClassName)
{
    CREATE_GAMEOBJECT(gameObjectClassName, GameObject);
    return new UIGameObject();
    /*
    CREATE_GAMEOBJECT(gameObjectClassName, UIGameObject);
    CREATE_GAMEOBJECT(gameObjectClassName, GUIMask);
    CREATE_GAMEOBJECT(gameObjectClassName, GUIButton);
    CREATE_GAMEOBJECT(gameObjectClassName, GUICanvas);
    CREATE_GAMEOBJECT(gameObjectClassName, GUIInputText);
    CREATE_GAMEOBJECT(gameObjectClassName, GUIScrollArea);
    CREATE_GAMEOBJECT(gameObjectClassName, GUITextCursor);

    Debug_Error("Please register class '" << gameObjectClassName << "' in "
                "GameObjectFactory"); ASSERT(false);

                */
    return nullptr;
}

#define EXISTS_GAMEOBJECT(gameObjectClassName, GameObjectClass) \
    if (gameObjectClassName == GameObjectClass::GetClassNameStatic()) \
    { return true; }

bool GameObjectFactory::ExistsGameObjectClass(const String &gameObjectClassName)
{
    EXISTS_GAMEOBJECT(gameObjectClassName, GameObject);
    EXISTS_GAMEOBJECT(gameObjectClassName, UIGameObject);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIMask);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIButton);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUICanvas);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIInputText);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIScrollArea);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUITextCursor);

    return false;
}

UIGameObject *GameObjectFactory::CreateGUIInputText()
{
    return GUIInputText::CreateGameObject();
}

UIGameObject *GameObjectFactory::CreateGUIButton()
{
    return GUIButton::CreateGameObject();
}

UIGameObject *GameObjectFactory::CreateGUILabel(const String &content)
{
    UIGameObject *label = new UIGameObject("GUILabel");

    UIGameObject *mask = new UIGameObject();
    mask->SetName("GUILabel_Mask");
    mask->AddComponent<GUIMask>();
    mask->AddComponent<UIImageRenderer>(); // Quad mask
    mask->SetParent(label);

    UIGameObject *textContainer = new UIGameObject("GUILabel_TextContainer");
    textContainer->SetParent(mask);

    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetContent(content);
    text->SetWrapping(false);

    return label;
}

UIGameObject *GameObjectFactory::CreateGUIScrollArea()
{
    return GUIScrollArea::CreateGameObject();
}
