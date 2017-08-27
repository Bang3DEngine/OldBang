#include "Bang/GameObjectFactory.h"

#include "Bang/GUIMask.h"
#include "Bang/GUIImage.h"
#include "Bang/GUILabel.h"
#include "Bang/Material.h"
#include "Bang/GUIButton.h"
#include "Bang/GUICanvas.h"
#include "Bang/GameObject.h"
#include "Bang/GUIInputText.h"
#include "Bang/UIGameObject.h"
#include "Bang/GUIScrollArea.h"
#include "Bang/GUITextCursor.h"
#include "Bang/RectTransform.h"

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
    CREATE_GAMEOBJECT(gameObjectClassName, GUIImage);
    CREATE_GAMEOBJECT(gameObjectClassName, GUILabel);
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
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIImage);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUILabel);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIButton);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUICanvas);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIInputText);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUIScrollArea);
    EXISTS_GAMEOBJECT(gameObjectClassName, GUITextCursor);

    return false;
}

UIGameObject *GameObjectFactory::CreateGUIInputText()
{
    UIGameObject *go = new UIGameObject();

    UIImageRenderer *imgRenderer = go->AddComponent<UIImageRenderer>();
    imgRenderer->UseMaterialCopy();
    imgRenderer->GetMaterial()->SetDiffuseColor(Color::Gray * 2.0f);

    GUILabel *label = new GUILabel();
    label->SetName("GUIInputText_Label");
    label->GetMask()->SetMasking(false);
    label->rectTransform->SetMargins(5, 2, 5, 2);

    UIGameObject *selectionQuad = new UIGameObject("GUIInputText_SelectionQuad");
    UIImageRenderer *selectionImg = selectionQuad->AddComponent<UIImageRenderer>();
    selectionImg->UseMaterialCopy();
    selectionImg->GetMaterial()->SetDiffuseColor(Color::LightBlue);
    selectionQuad->SetParent(label, 0);

    GUIScrollArea *boxScrollArea = new GUIScrollArea();
    boxScrollArea->SetName("GUIInputText_BoxMask");
    boxScrollArea->SetMasking(true);
    boxScrollArea->AddChild(label);
    boxScrollArea->SetParent(go);

    GUITextCursor *cursor = new GUITextCursor();
    cursor->SetName("GUIInputText_GUITextCursor");
    cursor->SetParent(label);

    GUIInputText *inputText = new GUIInputText();
    go->AddComponent(inputText);
    go->SetDefaultFocusAction(FocusAction::TakeIt);

    inputText->Init();

    return go;
}
