#include "Bang/GameObjectFactory.h"

#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"
#include "Bang/UIInputText.h"
#include "Bang/UIBorderRect.h"
#include "Bang/UIGameObject.h"
#include "Bang/UIScrollArea.h"
#include "Bang/UITextCursor.h"
#include "Bang/RectTransform.h"
#include "Bang/UIButtonTinter.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

GameObject*
GameObjectFactory::CreateGameObject(const String &gameObjectClassName)
{
    if (gameObjectClassName == GameObject::GetClassNameStatic())
    {
        return GameObjectFactory::CreateGameObject(false);
    }
    return GameObjectFactory::CreateUIGameObject(false);
}

bool GameObjectFactory::ExistsGameObjectClass(const String &gameObjectClassName)
{
    return gameObjectClassName == GameObject::GetClassNameStatic() ||
           gameObjectClassName == UIGameObject::GetClassNameStatic();
}

GameObject *GameObjectFactory::CreateGameObject(bool addTransform)
{
    GameObject *go = new GameObject();
    if (addTransform) { go->AddComponent<Transform>(); }
    return go;
}

UIGameObject *GameObjectFactory::CreateUIGameObject(bool addRectTransform)
{
    UIGameObject *go = new UIGameObject();
    if (addRectTransform) { go->AddComponent<RectTransform>(); }
    return go;
}

UIGameObject *GameObjectFactory::CreateGUIInputText()
{
    return UIInputText::CreateGameObject();
}

UIGameObject *GameObjectFactory::CreateGUIButton()
{
    UIGameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->SetName("GUIButton");

    UIGameObject *bg = GameObjectFactory::CreateUIGameObject(true);
    bg->SetName("GUIButton_Background");
    UIImageRenderer *bgImg = bg->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);
    bg->SetParent(go);

    UIGameObject *label = GameObjectFactory::CreateGUILabel();
    label->SetName("GUIButton_Label");
    label->GetComponentInChildren<UITextRenderer>()->SetTextColor(Color::Black);
    label->SetParent(go);

    UIBorderRect *borderRect = go->AddComponent<UIBorderRect>();
    borderRect->SetLineColor(Color::Purple);

    UIButtonTinter *labelTinter = go->AddComponent<UIButtonTinter>();
    labelTinter->AddAgent(go);
    labelTinter->AddGameObjectToTint(label);
    labelTinter->SetIdleTintColor(label->
                                  GetComponentInChildren<UITextRenderer>()->
                                  GetTextColor());
    labelTinter->SetOverTintColor(Color::Black);
    labelTinter->SetPressedTintColor(Color::White);

    UIButtonTinter *bgTinter = go->AddComponent<UIButtonTinter>();
    bgTinter->AddGameObjectToTint(bg);
    bgTinter->SetIdleTintColor(bgImg->GetTint());
    bgTinter->SetOverTintColor(Color::Gray);
    bgTinter->SetPressedTintColor(Color::Black);
    bgTinter->AddAgent(go);

    go->AddComponent<UIButton>();

    return go;
}

UIGameObject *GameObjectFactory::CreateGUILabel(const String &content)
{
    UIGameObject *label = GameObjectFactory::CreateUIGameObject(true);
    label->SetName("GUILabel");

    UIGameObject *mask = GameObjectFactory::CreateUIGameObject(true);
    mask->SetName("Mask");
    mask->AddComponent<UIMask>();
    mask->AddComponent<UIImageRenderer>(); // Quad mask
    mask->SetParent(label);

    UIGameObject *textContainer = GameObjectFactory::CreateUIGameObject(true);
    textContainer->SetName("TextContainer");
    textContainer->SetParent(mask);

    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetContent(content);
    text->SetWrapping(false);

    return label;
}

UIGameObject *GameObjectFactory::CreateGUIScrollArea()
{
    return UIScrollArea::CreateGameObject();
}
