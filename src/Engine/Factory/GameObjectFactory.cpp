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
#include "Bang/UIInteractive.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UITintedInteractive.h"

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

UIButton* GameObjectFactory::CreateGUIButton()
{
    GameObject *container = GameObjectFactory::CreateUIGameObject();
    UIFrameLayout *fl = container->AddComponent<UIFrameLayout>();
    /*fl->SetPaddingBot(2);
    fl->SetPaddingTop(2);
    fl->SetPaddingRight(5);
    fl->SetPaddingLeft (5);
    */

    UIGameObject *go = GameObjectFactory::CreateUIGameObject(true);
    go->AddComponent<UIFrameLayout>();
    go->SetName("GUIButton");

    UIGameObject *bg = GameObjectFactory::CreateUIGameObject(true);
    bg->SetName("GUIButton_Background");
    UIImageRenderer *bgImg = go->AddComponent<UIImageRenderer>();
    bgImg->SetTint(Color::White);
    bg->SetParent(go);

    UIGameObject *label = GameObjectFactory::CreateGUILabel();
    label->SetName("GUIButton_Label");
    label->GetComponentInChildren<UITextRenderer>()->SetTextColor(Color::Black);
    label->SetParent(go);

    UIBorderRect *borderRect = go->AddComponent<UIBorderRect>();
    borderRect->SetLineColor(Color::Purple);

    UITintedInteractive *labelBTint = go->AddComponent<UITintedInteractive>();
    labelBTint->AddAgent(go);
    labelBTint->AddGameObjectToTint(label);
    labelBTint->SetIdleTintColor(label->
                                  GetComponentInChildren<UITextRenderer>()->
                                  GetTextColor());
    labelBTint->SetOverTintColor(Color::Black);
    labelBTint->SetPressedTintColor(Color::White);

    UITintedInteractive *bgWTint = go->AddComponent<UITintedInteractive>();
    bgWTint->AddGameObjectToTint(bg);
    bgWTint->SetIdleTintColor(bgImg->GetTint());
    bgWTint->SetOverTintColor(Color::Gray);
    bgWTint->SetPressedTintColor(Color::Black);
    bgWTint->AddAgent(go);

    UIButton *button = container->AddComponent<UIButton>();
    button->SetBackground(bgImg);
    button->SetBorder(borderRect);
    button->SetText(label->FindInChildren("TextContainer")->
                    GetComponent<UITextRenderer>());

    go->SetParent(container);

    return button;
}

UIGameObject *GameObjectFactory::CreateGUILabel(const String &content)
{
    UIGameObject *label = GameObjectFactory::CreateUIGameObject(true);
    label->SetName("GUILabel");
    label->AddComponent<UIFrameLayout>();
    label->AddComponent<UIMask>();
    label->AddComponent<UIImageRenderer>(); // Quad mask

    UIGameObject *textGo = GameObjectFactory::CreateUIGameObject(true);
    textGo->SetName("TextContainer");
    textGo->SetParent(label);

    UITextRenderer *text = textGo->AddComponent<UITextRenderer>();
    text->SetContent(content);
    text->SetWrapping(false);

    return label;
}

UIGameObject *GameObjectFactory::CreateGUIScrollArea()
{
    return UIScrollArea::CreateGameObject();
}
