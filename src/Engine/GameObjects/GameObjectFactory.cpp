#include "Bang/GameObjectFactory.h"

#include "Bang/UIMask.h"
#include "Bang/Material.h"
#include "Bang/UIButton.h"
#include "Bang/UICanvas.h"
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

GameObject*
GameObjectFactory::CreateGameObject(const String &gameObjectClassName)
{
    if (gameObjectClassName == GameObject::GetClassNameStatic())
    {
        return new GameObject();
    }
    return new UIGameObject();
}

bool GameObjectFactory::ExistsGameObjectClass(const String &gameObjectClassName)
{
    return gameObjectClassName == GameObject::GetClassNameStatic() ||
           gameObjectClassName == UIGameObject::GetClassNameStatic();
}

UIGameObject *GameObjectFactory::CreateGUIInputText()
{
    return UIInputText::CreateGameObject();
}

UIGameObject *GameObjectFactory::CreateGUIButton()
{
    UIGameObject *go = new UIGameObject("GUIButton");

    UIGameObject *bg = new UIGameObject("GUIButton_Background");
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
    UIGameObject *label = new UIGameObject("GUILabel");

    UIGameObject *mask = new UIGameObject();
    mask->SetName("Mask");
    mask->AddComponent<UIMask>();
    mask->AddComponent<UIImageRenderer>(); // Quad mask
    mask->SetParent(label);

    UIGameObject *textContainer = new UIGameObject("TextContainer");
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
